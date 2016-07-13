/** * Copyright (c) 2011 ~ 2012 Deepin, Inc.
 *               2011 ~ 2012 Long Wei
 *
 * Author:      Long Wei <yilang2007lw@gmail.com>
 *              bluth <yuanchenglu001@gmail.com>
 * Maintainer:  Long Wei <yilang2007lw@gamil.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see <http://www.gnu.org/licenses/>.
 **/

#include <sys/types.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <glib.h>
#include <glib/gkeyfile.h>
#include <gio/gio.h>
#include <stdio.h>
#include <string.h>
#include <shadow.h>
#include <unistd.h>
#include <errno.h>
#include <crypt.h>
#include <time.h>
#include <fcntl.h>
#include <sys/stat.h>

#include <security/pam_appl.h>
#include <security/pam_misc.h>

#define LigthDMGreeterConfig "/var/lib/lightdm/lightdm-deepin-greeter/state_user"
#define LOCK_DBUS_NAME     "com.deepin.dde.lock"
#define LOCK_DBUS_OBJ       "/com/deepin/dde/lock"
#define LOCK_DBUS_IFACE     "com.deepin.dde.lock"

struct LoginInfo {
    char *username;
    gboolean is_already_no_pwd_login;
};

static struct LoginInfo login_info = {NULL, FALSE};

const char *_lock_dbus_iface_xml =
    "<?xml version=\"1.0\"?>\n"
    "<node>\n"
    "	<interface name=\""LOCK_DBUS_IFACE"\">\n"
    "		<method name=\"SwitchToUser\">\n"
    "			<arg name=\"username\" type=\"s\" direction=\"in\">\n"
    "			</arg>\n"
    "			<arg name=\"succeed\" type=\"b\" direction=\"out\">\n"
    "			</arg>\n"
    "		</method>\n"
    "		<method name=\"CurrentUser\">\n"
    "			<arg name=\"username\" type=\"s\" direction=\"out\">\n"
    "			</arg>\n"
    "		</method>\n"
    "		<method name=\"UnlockCheck\">\n"
    "			<arg name=\"username\" type=\"s\" direction=\"in\">\n"
    "			</arg>\n"
    "			<arg name=\"password\" type=\"s\" direction=\"in\">\n"
    "			</arg>\n"
    "			<arg name=\"succeed\" type=\"b\" direction=\"out\">\n"
    "			</arg>\n"
    "		</method>\n"
    "		<method name=\"NeedPwd\">\n"
    "			<arg name=\"username\" type=\"s\" direction=\"in\">\n"
    "			</arg>\n"
    "			<arg name=\"needed\" type=\"b\" direction=\"out\">\n"
    "			</arg>\n"
    "		</method>\n"
    "		<method name=\"IsLiveCD\">\n"
    "			<arg name=\"username\" type=\"s\" direction=\"in\">\n"
    "			</arg>\n"
    "			<arg name=\"livecd\" type=\"b\" direction=\"out\">\n"
    "			</arg>\n"
    "		</method>\n"
    "		<method name=\"AddNoPwdLogin\">\n"
    "			<arg name=\"username\" type=\"s\" direction=\"in\">\n"
    "			</arg>\n"
    "			<arg name=\"result\" type=\"b\" direction=\"out\">\n"
    "			</arg>\n"
    "		</method>\n"
    "		<method name=\"RemoveNoPwdLogin\">\n"
    "			<arg name=\"username\" type=\"s\" direction=\"in\">\n"
    "			</arg>\n"
    "			<arg name=\"result\" type=\"b\" direction=\"out\">\n"
    "			</arg>\n"
    "		</method>\n"
    "       <signal name=\"UserUnlock\">"
    "           <arg type=\"s\" name=\"user\"></arg>"
    "       </signal>"
    "       <signal name=\"UserChanged\">"
    "           <arg type=\"s\" name=\"user\"></arg>"
    "       </signal>"
    "	</interface>\n"
    "</node>\n"
    ;

static GMainLoop *loop = NULL;
static guint lock_service_owner_id;
static guint lock_service_reg_id;    //used for unregister an object path
static guint retry_reg_timeout_id;   //timer used for retrying dbus name registration.
static GDBusConnection *lock_connection;

//internal functions
static gboolean _retry_registration(gpointer user_data);

static void _on_bus_acquired(GDBusConnection *connection, const gchar *name, gpointer user_data);

static void _on_name_acquired(GDBusConnection *connection, const gchar *name, gpointer user_data);

static void _on_name_lost(GDBusConnection *connection, const gchar *name, gpointer user_data);

static void _bus_method_call(GDBusConnection *connection, const gchar *sender, const gchar *object_path, const gchar *interface,
                             const gchar *method, GVariant *params, GDBusMethodInvocation *invocation, gpointer user_data);

static void _bus_handle_exit_lock(GDBusConnection *connection, const gchar *username, const gchar *password);

static gboolean _bus_handle_need_pwd(const gchar *username);

static gboolean _bus_handle_is_livecd(const gchar *username);

static gboolean _bus_handle_unlock_check(const gchar *username, const gchar *password);

static gboolean _bus_handle_add_nopwdlogin(const gchar *username);

static gboolean _bus_handle_remove_nopwdlogin(const gchar *username);

static gboolean do_exit(gpointer user_data);

static int check_user_conv(int num_msg, const struct pam_message **msgm,
                           struct pam_response **response, void *appdata_ptr);
static int check_user(const char *user, const char *passwd);

static GDBusNodeInfo       *node_info = NULL;
static GDBusInterfaceInfo *interface_info = NULL;
static GDBusInterfaceVTable interface_table = {
method_call:   _bus_method_call,
get_property:   NULL, /* No properties */
set_property:   NULL  /* No properties */
};

void
lock_setup_dbus_service()
{
    GError *error = NULL;

    node_info = g_dbus_node_info_new_for_xml(_lock_dbus_iface_xml, &error);
    if (error != NULL) {
        g_critical("Unable to parse interface xml: %s", error->message);
        g_error_free(error);
    }

    interface_info = g_dbus_node_info_lookup_interface(node_info, LOCK_DBUS_IFACE);
    if (interface_info == NULL) {
        g_critical("Unable to find interface '"LOCK_DBUS_IFACE"'");
    }

    lock_service_owner_id = 0;
    lock_service_reg_id = 0;
    retry_reg_timeout_id = 0;

    _retry_registration(NULL);
}

static gboolean
_retry_registration(gpointer user_data G_GNUC_UNUSED)
{
    lock_service_owner_id = g_bus_own_name(G_BUS_TYPE_SYSTEM,
                                           LOCK_DBUS_NAME,
                                           G_BUS_NAME_OWNER_FLAGS_NONE,
                                           lock_service_reg_id ? NULL : _on_bus_acquired,
                                           _on_name_acquired,
                                           _on_name_lost,
                                           NULL,
                                           NULL);
    return TRUE;
}

static void
_on_bus_acquired(GDBusConnection *connection, const gchar *name G_GNUC_UNUSED, gpointer user_data)
{
    GError *error = NULL;

    g_debug("on_bus_acquired");
    lock_connection = connection;

    //register object.
    lock_service_reg_id = g_dbus_connection_register_object(connection,
                          LOCK_DBUS_OBJ,
                          interface_info,
                          &interface_table,
                          user_data,
                          NULL,
                          &error);
    if (error != NULL) {
        g_critical("Unable to register the object to DBus: %s", error->message);

        g_error_free(error);

        g_bus_unown_name(lock_service_owner_id);

        lock_service_owner_id = 0;
        retry_reg_timeout_id = g_timeout_add_seconds(1, _retry_registration, NULL);

        return;
    }
    return;
}

static void
_on_name_acquired(GDBusConnection *connection G_GNUC_UNUSED,
                  const gchar *name G_GNUC_UNUSED,
                  gpointer user_data G_GNUC_UNUSED)
{
    g_debug("Dbus name acquired");
}

static void
_on_name_lost(GDBusConnection *connection, const gchar *name G_GNUC_UNUSED, gpointer user_data G_GNUC_UNUSED)
{
    if (connection == NULL) {
        g_critical("Unable to get a connection to DBus");

    } else {
        g_critical("Unable to claim the name %s", LOCK_DBUS_NAME);
    }

    lock_service_owner_id = 0;
}

/*
 *  this function implements all the methods in the Registrar interface.
 */
static void
_bus_method_call(GDBusConnection *connection G_GNUC_UNUSED,
                 const gchar *sender G_GNUC_UNUSED,
                 const gchar *object_path G_GNUC_UNUSED,
                 const gchar *interface G_GNUC_UNUSED,
                 const gchar *method,
                 GVariant *params,
                 GDBusMethodInvocation *invocation,
                 gpointer user_data G_GNUC_UNUSED)
{
    GVariant *retval = NULL;
    GError *error = NULL;

    if (g_strcmp0(method, "SwitchToUser") == 0) {
        const gchar *username = NULL;
        GKeyFile *key_file = NULL;
        int cfg = open(LigthDMGreeterConfig,  O_RDWR | O_CREAT, S_IRUSR | S_IRGRP | S_IROTH);
        close(cfg);

        g_variant_get(params, "(s)", &username);

        key_file = g_key_file_new();
        g_key_file_load_from_file(key_file,
                                  LigthDMGreeterConfig,
                                  G_KEY_FILE_NONE,
                                  &error);
        g_key_file_set_value(key_file,
                             "General",
                             "last-user",
                             username);
        g_key_file_save_to_file(key_file,
                                LigthDMGreeterConfig,
                                &error);
        g_key_file_free(key_file);

        g_dbus_connection_emit_signal(connection,
                                      NULL,
                                      LOCK_DBUS_OBJ,
                                      LOCK_DBUS_IFACE,
                                      "UserChanged",
                                      params,
                                      &error);
        retval = g_variant_new("(b)", TRUE);
    } else if (g_strcmp0(method, "CurrentUser") == 0) {
        gchar *username = NULL;
        GKeyFile *key_file = NULL;
        key_file = g_key_file_new();
        g_key_file_load_from_file(key_file,
                                  LigthDMGreeterConfig,
                                  G_KEY_FILE_NONE,
                                  &error);

        username = g_key_file_get_value(key_file,
                                        "General",
                                        "last-user",
                                        &error);
        g_key_file_free(key_file);
        retval = g_variant_new("(s)", username);
    } else if (g_strcmp0(method, "UnlockCheck") == 0) {

        const gchar *username = NULL;
        const gchar *password = NULL;
        g_variant_get(params, "(ss)", &username, &password);

        retval = g_variant_new("(b)", _bus_handle_unlock_check(username, password));

    } else if (g_strcmp0(method, "NeedPwd") == 0) {

        const gchar *username = NULL;
        g_variant_get(params, "(s)", &username);

        retval = g_variant_new("(b)", _bus_handle_need_pwd(username));

    } else if (g_strcmp0(method, "IsLiveCD") == 0) {

        const gchar *username = NULL;
        g_variant_get(params, "(s)", &username);

        retval = g_variant_new("(b)", _bus_handle_is_livecd(username));

    } else if (0 == g_strcmp0(method, "AddNoPwdLogin")) {
        const gchar *username = NULL;
        g_variant_get(params, "(s)", &username);

        retval = g_variant_new("(b)", _bus_handle_add_nopwdlogin(username));

    } else if (0 == g_strcmp0(method, "RemoveNoPwdLogin")) {
        const gchar *username = NULL;
        g_variant_get(params, "(s)", &username);

        retval = g_variant_new("(b)", _bus_handle_remove_nopwdlogin(username));

    } else {

        g_warning("Calling method '%s' on lock and it's unknown", method);
    }

    if (error != NULL) {

        g_dbus_method_invocation_return_dbus_error(invocation, "com.deepin.dde.lock.Error", error->message);
        g_error_free(error);

    } else {
        g_dbus_method_invocation_return_value(invocation, retval);
    }
}

static void
_bus_handle_exit_lock(GDBusConnection *connection, const gchar *username, const gchar *password)
{
    GVariant *arg = NULL;
    GError *error = NULL;
    gboolean succeed = _bus_handle_unlock_check(username, password);

    time_t timer;
    char buffer[26];
    struct tm *tm_info;

    time(&timer);
    tm_info = localtime(&timer);

    strftime(buffer, 26, "%Y:%m:%d %H:%M:%S", tm_info);

    printf("exit lock %s\n", buffer);
    if (!succeed) {
        g_debug("username and password not match");
        goto _bus_handle_exit_lock_out;
    }
    // TODO: donot use the fuck dbus

    gchar *add_cmd = g_strdup_printf("killall dde-lock");
    g_spawn_command_line_sync(add_cmd, NULL, NULL, NULL, &error);
    if (error != NULL) {
        printf("g_spawn_command_line_sync:%s\n", error->message);
        g_error_free(error);
    }
    error = NULL;

    arg = g_variant_new("(s)", username);
    g_dbus_connection_emit_signal(connection,
                                  NULL,
                                  LOCK_DBUS_OBJ,
                                  LOCK_DBUS_IFACE,
                                  "UserUnlock",
                                  arg,
                                  &error);
    // check user is lock or not
_bus_handle_exit_lock_out:
    printf("exit lock of %s\n", username);
}

static gboolean
_bus_handle_unlock_check(const gchar *username, const gchar *password)
{
    if (!(_bus_handle_need_pwd(username))) {
        return TRUE;
    }

    int ret = check_user(username, password);
    if (ret != 0) {
        return FALSE;
    }
    return TRUE;
}

static int check_user_conv(int num_msg, const struct pam_message **msgm,
                           struct pam_response **response, void *appdata_ptr)
{
    struct pam_response *aresp;
    char *passwd = (char *)appdata_ptr;
    int i = 0;

    aresp = malloc(sizeof(struct pam_response) * num_msg);
    if (!aresp) {
        return PAM_CONV_ERR;
    }

    for (; i < num_msg; i++) {
        switch (msgm[i]->msg_style) {
        case PAM_PROMPT_ECHO_OFF:
            if (passwd) {
                aresp[i].resp = g_strdup(passwd);
            }
            break;
        case PAM_PROMPT_ECHO_ON:
            if (passwd) {
                aresp[i].resp = g_strdup(passwd);
            }
            break;
        default:
            return PAM_CONV_ERR;
        }
    }
    *response = aresp;
    return PAM_SUCCESS;
}

static int check_user(const char *user, const char *passwd)
{
    if (!user || !passwd) {
        fprintf(stderr, "[check_user] empty user or passwd\n");
        return -1;
    }

    int retval;
    pam_handle_t *pamh = NULL;
    struct pam_conv conv = {
        check_user_conv,
        (void *)passwd
    };

    retval = pam_start("lightdm", user, &conv, &pamh);
    if (retval != PAM_SUCCESS) {
        fprintf(stderr, "[check_user] pam_start: %s\n", pam_strerror(pamh, retval));
        return -1;
    }

    // check passwd whether right
    retval = pam_authenticate(pamh, 0);

    if (retval != PAM_SUCCESS) {
        printf("[check_user] pam_authenticate: %s\n", pam_strerror(pamh, retval));
        goto out;
    }

    // check user account whether overdue
    retval = pam_acct_mgmt(pamh, 0);
    if (retval != PAM_SUCCESS) {
        fprintf(stderr, "[check_user] pam_acct_mgmt: %s\n", pam_strerror(pamh, retval));
        goto out;
    }

out:
    if (pam_end(pamh, retval) != PAM_SUCCESS) {
        pamh = NULL;
        fprintf(stderr, "[check_user] pam_end: %s\n", pam_strerror(pamh, retval));
        return -1;;
    }

    return (retval == PAM_SUCCESS) ? 0 : -1;
}

static
GPtrArray *get_nopasswdlogin_users()
{
    GPtrArray *nopasswdlogin = g_ptr_array_new();
    GError *error = NULL;

    GFile *file = g_file_new_for_path("/etc/group");
    g_assert(file);

    GFileInputStream *input = g_file_read(file, NULL, &error);
    if (error != NULL) {
        g_warning("read /etc/group failed\n");
        g_clear_error(&error);
    }
    g_assert(input);

    GDataInputStream *data_input = g_data_input_stream_new((GInputStream *) input);
    g_assert(data_input);

    char *data = (char *) 1;
    while (data) {
        gsize length = 0;
        data = g_data_input_stream_read_line(data_input, &length, NULL, &error);
        if (error != NULL) {
            g_warning("read line error\n");
            g_clear_error(&error);
        }

        if (data != NULL) {
            if (g_str_has_prefix(data, "nopasswdlogin")) {
                gchar **nopwd_line = g_strsplit(data, ":", 4);
                g_debug("data:%s", data);
                g_debug("nopwd_line[3]:%s", nopwd_line[3]);

                if (nopwd_line[3] != NULL) {
                    gchar **user_strv = g_strsplit(nopwd_line[3], ",", 1024);

                    for (guint i = 0; i < g_strv_length(user_strv); i++) {
                        g_debug("user_strv[i]:%s", user_strv[i]);
                        g_ptr_array_add(nopasswdlogin, g_strdup(user_strv[i]));
                    }
                    g_strfreev(user_strv);
                }
                g_strfreev(nopwd_line);
            }
        } else {
            break;
        }
    }

    g_object_unref(data_input);
    g_object_unref(input);
    g_object_unref(file);

    return nopasswdlogin;
}

static
gboolean is_user_nopasswdlogin(const gchar *username)
{
    gboolean ret = FALSE;
    GPtrArray *nopwdlogin = get_nopasswdlogin_users();

    for (guint i = 0; i < nopwdlogin->len; i++) {
        g_debug("array i:%s", (gchar *) g_ptr_array_index(nopwdlogin, i));

        if (g_strcmp0(username, g_ptr_array_index(nopwdlogin, i)) == 0) {
            g_debug("nopwd login true");
            ret = TRUE;
        }
    }

    g_ptr_array_free(nopwdlogin, TRUE);

    return ret;
}

static gboolean
_bus_handle_need_pwd(const gchar *username)
{
    struct spwd *user_data;

    user_data = getspnam(username);

    if (user_data != NULL && strlen(user_data->sp_pwdp) == 0) {
        g_debug("user had blank password\n");
        return FALSE;
    }

    if (is_user_nopasswdlogin(username)) {
        g_debug("user in nopasswdlogin group\n");
        return FALSE;
    }

    if ((strcmp(crypt("", user_data->sp_pwdp), user_data->sp_pwdp)) == 0) {
        g_debug("live account don't need password\n");
        return FALSE;
    }

    return TRUE;
}

static gboolean
_bus_handle_is_livecd(const gchar *username)
{
    if (g_strcmp0("deepin", username) != 0) {
        return FALSE;
    }

    struct spwd *user_data;

    user_data = getspnam(username);

    if (user_data == NULL || strlen(user_data->sp_pwdp) == 0) {
        return FALSE;
    }

    if ((strcmp(crypt("", user_data->sp_pwdp), user_data->sp_pwdp)) != 0) {
        return FALSE;
    }

    return TRUE;
}

static gboolean
_bus_handle_add_nopwdlogin(const gchar *username)
{
    gboolean ret = FALSE;

    GError *error = NULL;

    if (is_user_nopasswdlogin(username)) {

        login_info.username = g_strdup(username);
        login_info.is_already_no_pwd_login = TRUE;
        ret = TRUE;

    } else {
        gchar *add_cmd = g_strdup_printf("gpasswd -a %s nopasswdlogin", username);

        g_spawn_command_line_sync(add_cmd, NULL, NULL, NULL, &error);
        if (error != NULL) {
            g_warning("_bus_handle_add_nopwdlogin:%s\n", error->message);
            g_error_free(error);

        } else {
            ret = TRUE;
        }
        error = NULL;

        g_free(add_cmd);
    }

    return ret;
}

static gboolean
_bus_handle_remove_nopwdlogin(const gchar *username)
{
    gboolean ret = FALSE;

    if (login_info.is_already_no_pwd_login) {
        return TRUE;
    }

    GError *error = NULL;

    if (!is_user_nopasswdlogin(username)) {

        ret = TRUE;

    } else {
        gchar *remove_cmd = g_strdup_printf("gpasswd -d %s nopasswdlogin", username);

        g_spawn_command_line_sync(remove_cmd, NULL, NULL, NULL, &error);
        if (error != NULL) {
            g_warning("_bus_handle_remove_nopwdlogin:%s\n", error->message);
            g_error_free(error);

        } else {
            ret = TRUE;
        }
        error = NULL;

        g_free(remove_cmd);
    }

    return ret;
}

static gboolean
do_exit(gpointer user_data G_GNUC_UNUSED)
{
    g_main_loop_quit(loop);

    return FALSE;
}

int main(int argc G_GNUC_UNUSED, char **argv G_GNUC_UNUSED)
{
    printf("start");
    fflush(stdout);
    loop = g_main_loop_new(NULL, FALSE);

    lock_setup_dbus_service();

    g_timeout_add_seconds(600000, do_exit, NULL);

    g_main_loop_run(loop);

    return 0;
}

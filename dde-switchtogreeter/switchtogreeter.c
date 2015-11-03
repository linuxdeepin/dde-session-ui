/**
 * Copyright (c) 2011 ~ 2013 Deepin, Inc.
 *               2011 ~ 2013 Long Wei
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

#include <glib.h>
#include <gio/gio.h>
#include <unistd.h>
#include <pwd.h>
#include <sys/types.h>

gchar *
get_user_name ()
{
    gchar *username = NULL;

    struct passwd *pw = NULL;
    pw = getpwuid (getuid ());
    if (pw != NULL) {
        username = g_strdup (pw->pw_name);

    } else {
        g_warning ("switchtogreeter:get user name failed\n");
    }

    return username;
}

gchar *
get_seat_path (gchar *username)
{
    gchar *seat_path = NULL;

    GDBusProxy *display_proxy = NULL;
    GVariant *sessions_prop_var = NULL;
    const gchar **sessions = NULL;
    gsize length = 0;
    GError *error = NULL;

    display_proxy = g_dbus_proxy_new_for_bus_sync (G_BUS_TYPE_SYSTEM,
                                                   G_DBUS_PROXY_FLAGS_NONE,
                                                   NULL,
                                                   "org.freedesktop.DisplayManager",
                                                   "/org/freedesktop/DisplayManager",
                                                   "org.freedesktop.DisplayManager",
                                                   NULL,
                                                   &error);
    if (error != NULL) {
        g_warning ("switchtogreeter:lightdm dbus %s\n", error->message);
        g_error_free (error);
        return seat_path;
    }
    error = NULL;

    sessions_prop_var = g_dbus_proxy_get_cached_property (display_proxy, "Sessions");
    if (sessions_prop_var == NULL) {
        g_warning ("switchtogreeter:get sessions list failed\n");
        g_object_unref (display_proxy);
        return seat_path;
    }
    g_object_unref(display_proxy);

    sessions = g_variant_get_objv (sessions_prop_var, &length);
    if (sessions == NULL) {
        g_warning ("switchtogreeter:parse sessions list failed\n");
        g_variant_unref (sessions_prop_var);
        return seat_path;
    }

    for (guint i = 0; i < length; ++i) {

        GDBusProxy *session_proxy = NULL;
        GVariant *username_prop_var = NULL;
        gchar *user_name = NULL;

        session_proxy = g_dbus_proxy_new_for_bus_sync (G_BUS_TYPE_SYSTEM,
                                                       G_DBUS_PROXY_FLAGS_NONE,
                                                       NULL,
                                                       "org.freedesktop.DisplayManager",
                                                       sessions[i],
                                                       "org.freedesktop.DisplayManager.Session",
                                                       NULL,
                                                       &error);
        if (error != NULL) {
            g_warning ("switchtogreeter:lightdm session dbus %s\n", error->message);
            g_error_free (error);
            continue;
        }
        error = NULL;

        username_prop_var = g_dbus_proxy_get_cached_property (session_proxy, "UserName");
        if (username_prop_var == NULL) {
            g_warning ("switchtogreeter:get username %s\n", error->message);
            g_error_free (error);
            g_object_unref (session_proxy);
            continue;
        }

        user_name = g_variant_dup_string (username_prop_var, NULL);
        if (user_name == NULL) {
            g_warning ("switchtogreeter:get username from session failed\n");
            g_object_unref (session_proxy);
            g_variant_unref (username_prop_var);
            continue;
        }

        if (g_strcmp0 (username, user_name) == 0) {
            GVariant *seat_prop_var = NULL;
            gsize seat_length = 0;

            seat_prop_var = g_dbus_proxy_get_cached_property (session_proxy, "Seat");
            if (seat_prop_var != NULL) {
                seat_path = g_variant_dup_string (seat_prop_var, &seat_length);

            } else {
                g_warning ("switchtogreeter:get seat path failed for user %s\n", username);
            }
        } else {
            continue;
        }

        g_free (user_name);
        g_variant_unref (username_prop_var);
        g_object_unref (session_proxy);
    }

    g_variant_unref (sessions_prop_var);
    g_free (sessions);

    return seat_path;
}

void
switch_to_greeter (gchar *seat_path)
{
    GDBusProxy *seat_proxy = NULL;
    GError *error = NULL;

    if (!g_variant_is_object_path (seat_path)) {
        g_warning ("switchtogreeter:invalid object path\n");
        return ;
    }

    seat_proxy = g_dbus_proxy_new_for_bus_sync (G_BUS_TYPE_SYSTEM,
                                                G_DBUS_PROXY_FLAGS_NONE,
                                                NULL,
                                                "org.freedesktop.DisplayManager",
                                                seat_path,
                                                "org.freedesktop.DisplayManager.Seat",
                                                NULL,
                                                &error);

    if (error != NULL) {
        g_warning ("switchtogreeter:seat proxy %s\n", error->message);
        g_error_free (error);
        return ;
    }
    error = NULL;

    g_dbus_proxy_call_sync (seat_proxy,
                            "SwitchToGreeter",
                            g_variant_new("()"),
                            G_DBUS_CALL_FLAGS_NONE,
                            -1,
                            NULL,
                            &error);

    if (error != NULL) {
        g_warning ("switchtogreeter:seat switchtogreeter %s\n", error->message);
        g_error_free (error);
    }
    error = NULL;

    g_object_unref (seat_proxy);
}

int main (int argc G_GNUC_UNUSED, char **argv G_GNUC_UNUSED)
{
    gchar *username = NULL;
    gchar *seat_path = NULL;

    username = get_user_name ();
    if (username == NULL) {
        g_warning ("switch to greeter:username is NULL\n");
        return 1;
    }

    seat_path = get_seat_path (username);
    if (seat_path == NULL) {
        g_warning ("switch to greeter:seat path is NULL\n");
        g_free (username);
        return 1;
    }

    switch_to_greeter (seat_path);

    g_free (username);
    g_free (seat_path);

    return 0;
}


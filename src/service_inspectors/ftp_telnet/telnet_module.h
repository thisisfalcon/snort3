/*
 * Copyright (C) 2014 Cisco and/or its affiliates. All rights reserved.
 **
 ** This program is free software; you can redistribute it and/or modify
 ** it under the terms of the GNU General Public License Version 2 as
 ** published by the Free Software Foundation.  You may not use, modify or
 ** distribute this program under any other version of the GNU General
 ** Public License.
 **
 ** This program is distributed in the hope that it will be useful,
 ** but WITHOUT ANY WARRANTY; without even the implied warranty of
 ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 ** GNU General Public License for more details.
 **
 ** You should have received a copy of the GNU General Public License
 ** along with this program; if not, write to the Free Software
 ** Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

// telnet_module.h author Russ Combs <rucombs@cisco.com>

#ifndef TELNET_MODULE_H
#define TELNET_MODULE_H

#include "ftpp_ui_config.h"
#include "framework/module.h"
#include "main/thread.h"

#define GID_TELNET 126

#define TELNET_AYT_OVERFLOW   1
#define TELNET_ENCRYPTED      2
#define TELNET_SB_NO_SE       3

#define TEL_NAME "telnet"
#define TEL_HELP "telnet inspection and normalization"

struct SnortConfig;

extern THREAD_LOCAL SimpleStats tnstats;
extern THREAD_LOCAL ProfileStats telnetPerfStats;

class TelnetModule : public Module
{
public:
    TelnetModule();
    ~TelnetModule();

    bool set(const char*, Value&, SnortConfig*);
    bool begin(const char*, int, SnortConfig*);
    bool end(const char*, int, SnortConfig*);

    unsigned get_gid() const
    { return GID_TELNET; };

    const RuleMap* get_rules() const;
    const char** get_pegs() const;
    PegCount* get_counts() const;
    ProfileStats* get_profile() const;

    TELNET_PROTO_CONF* get_data();

private:
    TELNET_PROTO_CONF* conf;
};

#endif

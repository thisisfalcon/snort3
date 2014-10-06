/*
** Copyright (C) 2014 Cisco and/or its affiliates. All rights reserved.
 * Copyright (C) 2002-2013 Sourcefire, Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License Version 2 as
 * published by the Free Software Foundation.  You may not use, modify or
 * distribute this program under any other version of the GNU General
 * Public License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */
// pps_frag3_engine.cc author Josh Rosenbaum <jrosenba@cisco.com>

#include <sstream>
#include <vector>

#include "conversion_state.h"
#include "utils/converter.h"
#include "utils/s2l_util.h"

namespace preprocessors
{

namespace {

class Frag3Engine : public ConversionState
{
public:
    explicit Frag3Engine() : ConversionState() {};
    virtual ~Frag3Engine() {};
    virtual bool convert(std::istringstream& data_stream);

private:
    bool parse_ip_list(std::string, std::istringstream& data_stream);
};

} // namespace


bool Frag3Engine::parse_ip_list(std::string list_name, 
                                std::istringstream& data_stream)
{
    std::string prev;
    std::string elem;

    if(!(data_stream >> elem) || (elem.front() != '['))
        return false;

    if(!(data_stream >> elem))
        return false;

    // there can be no spaces between the square bracket and string
    prev = "[" + elem;

    while (data_stream >> elem && elem.back() != ']')
        prev = prev + ' ' + elem;

    prev = prev + "]";
    return table_api.add_option(list_name, prev);

}

bool Frag3Engine::convert(std::istringstream& data_stream)
{

    bool retval = true;
    std::string keyword;

    table_api.open_table("stream_ip");

    while(data_stream >> keyword)
    {
        bool tmpval = true;

        if(keyword.back() == ',')
            keyword.pop_back();
        
        if(keyword.empty())
            continue;
        
        if(!keyword.compare("min_ttl"))
            tmpval = parse_int_option("min_ttl", data_stream);

        else if(!keyword.compare("detect_anomalies"))
            table_api.add_deleted_comment("detect_anomalies");

        else if(!keyword.compare("bind_to"))
            parse_ip_list("bind_to", data_stream);

        else if(!keyword.compare("overlap_limit"))
        {
            tmpval = parse_int_option("max_overlaps", data_stream);
            table_api.add_diff_option_comment("overlap_limit", "max_overlaps");
        }

        else if(!keyword.compare("min_fragment_length"))
        {
            tmpval = parse_int_option("min_frag_length", data_stream);
            table_api.add_diff_option_comment("min_fragment_length", "min_frag_length");
        }

        else if(!keyword.compare("timeout"))
        {
            std::string val;
            table_api.add_diff_option_comment("timeout", "session_timeout");

            if (data_stream >> val)
            {
                int seconds = std::stoi(val);
                if (seconds == 0)
                {
                    tmpval = table_api.add_option("session_timeout", 256);
                    table_api.add_diff_option_comment("preprocessor frag3_engine: timeout 0", "session_timeout 256");
                }
                else
                {
                    tmpval = table_api.add_option("session_timeout", seconds);
                }
            }
        }


        else if(!keyword.compare("policy"))
        {
            std::string policy;

            if (!(data_stream >> policy))
                tmpval = false;

            else if (!policy.compare("first"))
                tmpval = table_api.add_option("policy", "first");

            else if (!policy.compare("bsd"))
                tmpval = table_api.add_option("policy", "bsd");

            else if (!policy.compare("last"))
                tmpval = table_api.add_option("policy", "last");

            else if (!policy.compare("windows"))
                tmpval = table_api.add_option("policy", "windows");

            else if (!policy.compare("linux"))
                tmpval = table_api.add_option("policy", "linux");

            else if (!policy.compare("solaris"))
                tmpval = table_api.add_option("policy", "solaris");

            else if (!policy.compare("bsd-right"))
            {
                table_api.add_diff_option_comment("policy bsd-right", "policy = bsd_right");
                tmpval = table_api.add_option("policy", "bsd_right");
            }

            else
            {
                tmpval = false;
            }
        }

        else
            tmpval = false;

        if (retval)
            retval = tmpval;
    }

    return retval;    
}

/**************************
 *******  A P I ***********
 **************************/

static ConversionState* ctor()
{
    return new Frag3Engine();
}

static const ConvertMap preprocessor_frag3_engine =
{
    "frag3_engine",
    ctor,
};

const ConvertMap* frag3_engine_map = &preprocessor_frag3_engine;

} // namespace preprocessors
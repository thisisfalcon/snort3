/*
** Copyright (C) 2002-2013 Sourcefire, Inc.
** Copyright (C) 1998-2002 Martin Roesch <roesch@sourcefire.com>
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



#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "framework/codec.h"
#include "codecs/codec_events.h"

// yes, macros are necessary. The API and class constructor require different strings.
#define CD_MOBILE_NAME "mobility"
#define CD_MOBILE_HELP "support for mobility"

namespace
{

class MobilityCodec : public Codec
{
public:
    MobilityCodec() : Codec(CD_MOBILE_NAME){};
    ~MobilityCodec() {};


    virtual void get_protocol_ids(std::vector<uint16_t>&);
    virtual bool decode(const RawData&, CodecData&, DecodeData&);
};

} // namespace


void MobilityCodec::get_protocol_ids(std::vector<uint16_t>& v)
{
    v.push_back(IPPROTO_ID_MOBILITY);
}

bool MobilityCodec::decode(const RawData&, CodecData& codec, DecodeData&)
{
    codec_events::decoder_event(codec, DECODE_IP_BAD_PROTO);
    codec.proto_bits |= PROTO_BIT__IP6_EXT; // check for any IP related rules
    return true;
}


//-------------------------------------------------------------------------
// api
//-------------------------------------------------------------------------

static Codec* ctor(Module*)
{ return new MobilityCodec(); }

static void dtor(Codec *cd)
{ delete cd; }


static const CodecApi mobility_api =
{
    {
        PT_CODEC,
        CD_MOBILE_NAME,
        CD_MOBILE_HELP,
        CDAPI_PLUGIN_V0,
        0,
        nullptr,
        nullptr
    },
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    ctor,
    dtor,
};


#ifdef BUILDING_SO
SO_PUBLIC const BaseApi* snort_plugins[] =
{
    &mobility_api.base,
    nullptr
};
#else
const BaseApi* cd_mobility = &mobility_api.base;
#endif
/*
   The MIT License (MIT)

   Copyright (c) 2021 Doerthous

   Permission is hereby granted, free of charge, to any person obtaining a
   copy of this software and associated documentation files (the "Software"),
   to deal in the Software without restriction, including without limitation
   the rights to use, copy, modify, merge, publish, distribute, sublicense,
   and/or sell copies of the Software, and to permit persons to whom the
   Software is furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included in
   all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
   OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
   FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
   DEALINGS IN THE SOFTWARE.

   Authour: Doerthous <doerthous@gmail.com>
*/

#ifndef DTS_NET_CONF_H_
#define DTS_NET_CONF_H_

#define DTS_NET_ARP_TABLE_MAX_SIZE      1
#define DTS_NET_NET_IF_MAX_COUNT        1
#define DTS_NET_SERVER_UDP_MAX_COUNT    1
#define DTS_NET_IP_DATAGRAM_MAX_COUNT   1
#define DTS_NET_SYS_HEARTBEAT_FREQ      1

#include <rtthread.h>
#define dts_net_malloc rt_malloc
#define dts_net_free rt_free
#define mem_size_t rt_size_t

#endif // DTS_NET_CONF_H_

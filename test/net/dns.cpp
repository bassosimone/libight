/*-
 * This file is part of Libight <https://libight.github.io/>.
 *
 * Libight is free software. See AUTHORS and LICENSE for more
 * information on the copying conditions.
 */

//
// Regression tests for `net/dns.hpp`
//

#define CATCH_CONFIG_MAIN
#include "src/ext/Catch/single_include/catch.hpp"

#include "net/dns.hpp"

#include "common/log.h"

TEST_CASE("The system resolver works as expected") {

    auto d = IghtDelayedCall(10.0, [](void) {
        throw std::runtime_error("Test failed");
    });

    auto r1 = ight::DNSRequest("A", "www.neubot.org", [&](
                               ight::DNSResponse&& response) {
        REQUIRE(response.get_query_name() == "www.neubot.org");
        REQUIRE(response.get_query_type() == "A");
        REQUIRE(response.get_query_class() == "IN");
        REQUIRE(response.get_reply_authoritative() == "unknown");
        REQUIRE(response.get_resolver()[0] == "<default>");
        REQUIRE(response.get_resolver()[1] == "53");
        REQUIRE(response.get_evdns_status() == DNS_ERR_NONE);
        REQUIRE(response.get_failure() == "");
        REQUIRE(response.get_results().size() == 1);
        REQUIRE(response.get_results()[0] == "130.192.16.172");
        REQUIRE(response.get_rtt() > 0.0);
        REQUIRE(response.get_ttl() > 0);
        ight_break_loop();
    });
    ight_loop();

    auto r2 = ight::DNSRequest("REVERSE_A", "130.192.16.172", [&](
                               ight::DNSResponse&& response) {
        REQUIRE(response.get_query_name() == "130.192.16.172");
        REQUIRE(response.get_query_type() == "PTR");
        REQUIRE(response.get_query_class() == "IN");
        REQUIRE(response.get_reply_authoritative() == "unknown");
        REQUIRE(response.get_resolver()[0] == "<default>");
        REQUIRE(response.get_resolver()[1] == "53");
        REQUIRE(response.get_evdns_status() == DNS_ERR_NONE);
        REQUIRE(response.get_failure() == "");
        REQUIRE(response.get_results().size() == 1);
        REQUIRE(response.get_results()[0] == "server-nexa.polito.it");
        REQUIRE(response.get_rtt() > 0.0);
        REQUIRE(response.get_ttl() > 0);
        ight_break_loop();
    });
    ight_loop();

    auto r3 = ight::DNSRequest("AAAA", "ooni.torproject.org", [&](
                               ight::DNSResponse&& response) {
        REQUIRE(response.get_query_name() == "ooni.torproject.org");
        REQUIRE(response.get_query_type() == "AAAA");
        REQUIRE(response.get_query_class() == "IN");
        REQUIRE(response.get_reply_authoritative() == "unknown");
        REQUIRE(response.get_resolver()[0] == "<default>");
        REQUIRE(response.get_resolver()[1] == "53");
        REQUIRE(response.get_evdns_status() == DNS_ERR_NONE);
        REQUIRE(response.get_failure() == "");
        REQUIRE(response.get_results().size() > 0);
        REQUIRE(response.get_rtt() > 0.0);
        REQUIRE(response.get_ttl() > 0);
        auto found = false;
        for (auto address : response.get_results()) {
            if (address == "2001:858:2:2:aabb:0:563b:1e28" ||
                address == "2001:858:2:2:aabb::563b:1e28") {
                found = true;
            }
        }
        REQUIRE(found);
        ight_break_loop();
    });
    ight_loop();

    auto r4 = ight::DNSRequest("REVERSE_AAAA", "2001:858:2:2:aabb:0:563b:1e28",
                               [&](ight::DNSResponse&& response) {
        REQUIRE(response.get_query_name() == "2001:858:2:2:aabb:0:563b:1e28");
        REQUIRE(response.get_query_type() == "PTR");
        REQUIRE(response.get_query_class() == "IN");
        REQUIRE(response.get_reply_authoritative() == "unknown");
        REQUIRE(response.get_resolver()[0] == "<default>");
        REQUIRE(response.get_resolver()[1] == "53");
        REQUIRE(response.get_evdns_status() == DNS_ERR_NONE);
        REQUIRE(response.get_failure() == "");
        REQUIRE(response.get_results().size() == 1);
        REQUIRE(response.get_results()[0] == "nova.torproject.org");
        REQUIRE(response.get_rtt() > 0.0);
        REQUIRE(response.get_ttl() > 0);
        ight_break_loop();
    });
    ight_loop();
}

TEST_CASE("The default custom resolver works as expected") {

    auto d = IghtDelayedCall(10.0, [](void) {
        throw std::runtime_error("Test failed");
    });

    auto reso = ight::DNSResolver();

    auto r1 = reso.request("A", "www.neubot.org", [&](
                           ight::DNSResponse&& response) {
        REQUIRE(response.get_query_name() == "www.neubot.org");
        REQUIRE(response.get_query_type() == "A");
        REQUIRE(response.get_query_class() == "IN");
        REQUIRE(response.get_reply_authoritative() == "unknown");
        REQUIRE(response.get_resolver()[0] == "<default>");
        REQUIRE(response.get_resolver()[1] == "53");
        REQUIRE(response.get_evdns_status() == DNS_ERR_NONE);
        REQUIRE(response.get_failure() == "");
        REQUIRE(response.get_results().size() == 1);
        REQUIRE(response.get_results()[0] == "130.192.16.172");
        REQUIRE(response.get_rtt() > 0.0);
        REQUIRE(response.get_ttl() > 0);
        ight_break_loop();
    });
    ight_loop();

    auto r2 = reso.request("REVERSE_A", "130.192.16.172", [&](
                           ight::DNSResponse&& response) {
        REQUIRE(response.get_query_name() == "130.192.16.172");
        REQUIRE(response.get_query_type() == "PTR");
        REQUIRE(response.get_query_class() == "IN");
        REQUIRE(response.get_reply_authoritative() == "unknown");
        REQUIRE(response.get_resolver()[0] == "<default>");
        REQUIRE(response.get_resolver()[1] == "53");
        REQUIRE(response.get_evdns_status() == DNS_ERR_NONE);
        REQUIRE(response.get_failure() == "");
        REQUIRE(response.get_results().size() == 1);
        REQUIRE(response.get_results()[0] == "server-nexa.polito.it");
        REQUIRE(response.get_rtt() > 0.0);
        REQUIRE(response.get_ttl() > 0);
        ight_break_loop();
    });
    ight_loop();

    auto r3 = reso.request("AAAA", "ooni.torproject.org", [&](
                           ight::DNSResponse&& response) {
        REQUIRE(response.get_query_name() == "ooni.torproject.org");
        REQUIRE(response.get_query_type() == "AAAA");
        REQUIRE(response.get_query_class() == "IN");
        REQUIRE(response.get_reply_authoritative() == "unknown");
        REQUIRE(response.get_resolver()[0] == "<default>");
        REQUIRE(response.get_resolver()[1] == "53");
        REQUIRE(response.get_evdns_status() == DNS_ERR_NONE);
        REQUIRE(response.get_failure() == "");
        REQUIRE(response.get_results().size() > 0);
        REQUIRE(response.get_rtt() > 0.0);
        REQUIRE(response.get_ttl() > 0);
        auto found = false;
        for (auto address : response.get_results()) {
            if (address == "2001:858:2:2:aabb:0:563b:1e28" ||
                address == "2001:858:2:2:aabb::563b:1e28") {
                found = true;
            }
        }
        REQUIRE(found);
        ight_break_loop();
    });
    ight_loop();

    auto r4 = reso.request("REVERSE_AAAA", "2001:858:2:2:aabb:0:563b:1e28",
                           [&](ight::DNSResponse&& response) {
        REQUIRE(response.get_query_name() == "2001:858:2:2:aabb:0:563b:1e28");
        REQUIRE(response.get_query_type() == "PTR");
        REQUIRE(response.get_query_class() == "IN");
        REQUIRE(response.get_reply_authoritative() == "unknown");
        REQUIRE(response.get_resolver()[0] == "<default>");
        REQUIRE(response.get_resolver()[1] == "53");
        REQUIRE(response.get_evdns_status() == DNS_ERR_NONE);
        REQUIRE(response.get_failure() == "");
        REQUIRE(response.get_results().size() == 1);
        REQUIRE(response.get_results()[0] == "nova.torproject.org");
        REQUIRE(response.get_rtt() > 0.0);
        REQUIRE(response.get_ttl() > 0);
        ight_break_loop();
    });
    ight_loop();
}

TEST_CASE("A specific custom resolver works as expected") {

    auto d = IghtDelayedCall(10.0, [](void) {
        throw std::runtime_error("Test failed");
    });

    auto reso = ight::DNSResolver("8.8.4.4");

    auto r1 = reso.request("A", "www.neubot.org", [&](
                           ight::DNSResponse&& response) {
        REQUIRE(response.get_query_name() == "www.neubot.org");
        REQUIRE(response.get_query_type() == "A");
        REQUIRE(response.get_query_class() == "IN");
        REQUIRE(response.get_reply_authoritative() == "unknown");
        REQUIRE(response.get_resolver()[0] == "8.8.4.4");
        REQUIRE(response.get_resolver()[1] == "53");
        REQUIRE(response.get_evdns_status() == DNS_ERR_NONE);
        REQUIRE(response.get_failure() == "");
        REQUIRE(response.get_results().size() == 1);
        REQUIRE(response.get_results()[0] == "130.192.16.172");
        REQUIRE(response.get_rtt() > 0.0);
        REQUIRE(response.get_ttl() > 0);
        ight_break_loop();
    });
    ight_loop();

    auto r2 = reso.request("REVERSE_A", "130.192.16.172", [&](
                           ight::DNSResponse&& response) {
        REQUIRE(response.get_query_name() == "130.192.16.172");
        REQUIRE(response.get_query_type() == "PTR");
        REQUIRE(response.get_query_class() == "IN");
        REQUIRE(response.get_reply_authoritative() == "unknown");
        REQUIRE(response.get_resolver()[0] == "8.8.4.4");
        REQUIRE(response.get_resolver()[1] == "53");
        REQUIRE(response.get_evdns_status() == DNS_ERR_NONE);
        REQUIRE(response.get_failure() == "");
        REQUIRE(response.get_results().size() == 1);
        REQUIRE(response.get_results()[0] == "server-nexa.polito.it");
        REQUIRE(response.get_rtt() > 0.0);
        REQUIRE(response.get_ttl() > 0);
        ight_break_loop();
    });
    ight_loop();

    auto r3 = reso.request("AAAA", "ooni.torproject.org", [&](
                           ight::DNSResponse&& response) {
        REQUIRE(response.get_query_name() == "ooni.torproject.org");
        REQUIRE(response.get_query_type() == "AAAA");
        REQUIRE(response.get_query_class() == "IN");
        REQUIRE(response.get_reply_authoritative() == "unknown");
        REQUIRE(response.get_resolver()[0] == "8.8.4.4");
        REQUIRE(response.get_resolver()[1] == "53");
        REQUIRE(response.get_evdns_status() == DNS_ERR_NONE);
        REQUIRE(response.get_failure() == "");
        REQUIRE(response.get_results().size() > 0);
        REQUIRE(response.get_rtt() > 0.0);
        REQUIRE(response.get_ttl() > 0);
        auto found = false;
        for (auto address : response.get_results()) {
            if (address == "2001:858:2:2:aabb:0:563b:1e28" ||
                address == "2001:858:2:2:aabb::563b:1e28") {
                found = true;
            }
        }
        REQUIRE(found);
        ight_break_loop();
    });
    ight_loop();

    auto r4 = reso.request("REVERSE_AAAA", "2001:858:2:2:aabb:0:563b:1e28",
                           [&](ight::DNSResponse&& response) {
        REQUIRE(response.get_query_name() == "2001:858:2:2:aabb:0:563b:1e28");
        REQUIRE(response.get_query_type() == "PTR");
        REQUIRE(response.get_query_class() == "IN");
        REQUIRE(response.get_reply_authoritative() == "unknown");
        REQUIRE(response.get_resolver()[0] == "8.8.4.4");
        REQUIRE(response.get_resolver()[1] == "53");
        REQUIRE(response.get_evdns_status() == DNS_ERR_NONE);
        REQUIRE(response.get_failure() == "");
        REQUIRE(response.get_results().size() == 1);
        REQUIRE(response.get_results()[0] == "nova.torproject.org");
        REQUIRE(response.get_rtt() > 0.0);
        REQUIRE(response.get_ttl() > 0);
        ight_break_loop();
    });
    ight_loop();
}

TEST_CASE("Cancel is idempotent") {

    auto r1 = ight::DNSRequest("A", "www.neubot.org", [&](
                               ight::DNSResponse&& /*response*/) {
        throw std::runtime_error("Test failed");
    });

    r1.cancel();
    r1.cancel();
    r1.cancel();
}

TEST_CASE("A request to a nonexistent server times out") {

    auto reso = ight::DNSResolver("130.192.91.231", "1");
    auto r1 = reso.request("A", "www.neubot.org", [&](
                           ight::DNSResponse&& response) {
        REQUIRE(response.get_query_name() == "www.neubot.org");
        REQUIRE(response.get_query_type() == "A");
        REQUIRE(response.get_query_class() == "IN");
        REQUIRE(response.get_reply_authoritative() == "unknown");
        REQUIRE(response.get_resolver()[0] == "130.192.91.231");
        REQUIRE(response.get_resolver()[1] == "53");
        REQUIRE(response.get_results().size() == 0);
        REQUIRE(response.get_evdns_status() == DNS_ERR_TIMEOUT);
        REQUIRE(response.get_failure() == "deferred_timeout_error");
        REQUIRE(response.get_ttl() == 0);
        REQUIRE(response.get_rtt() == 0.0);
        ight_break_loop();
    });

    auto d = IghtDelayedCall(10.0, [](void) {
        throw std::runtime_error("Test failed");
    });

    ight_loop();
}

TEST_CASE("If the resolver dies, the requests are aborted") {

    auto reso = new ight::DNSResolver("130.192.91.231");
    auto r1 = reso->request("A", "www.neubot.org", [&](
                            ight::DNSResponse&& response) {
        REQUIRE(response.get_query_name() == "www.neubot.org");
        REQUIRE(response.get_query_type() == "A");
        REQUIRE(response.get_query_class() == "IN");
        REQUIRE(response.get_reply_authoritative() == "unknown");
        REQUIRE(response.get_resolver()[0] == "130.192.91.231");
        REQUIRE(response.get_resolver()[1] == "53");
        REQUIRE(response.get_results().size() == 0);
        REQUIRE(response.get_evdns_status() == DNS_ERR_SHUTDOWN);
        REQUIRE(response.get_failure() == "unknown failure 68");
        REQUIRE(response.get_ttl() == 0);
        REQUIRE(response.get_rtt() == 0.0);
        ight_break_loop();
    });

    auto d1 = IghtDelayedCall(0.1, [&](void) {
        delete reso;  // Destroy the resolver and see what happens
    });
    auto d2 = IghtDelayedCall(1.0, [](void) {
        throw std::runtime_error("Test failed");
    });

    ight_loop();
}

TEST_CASE("It is safe to forget about pending requests") {

    {
        auto r1 = ight::DNSRequest("A", "www.neubot.org", [&](
                                   ight::DNSResponse&& /*response*/) {
            throw std::runtime_error("Should not happen");
        });

    }  // This should kill r1

    auto d = IghtDelayedCall(5.0, [](void) {
        ight_break_loop();  // We should receive a response earlier than this
    });

    ight_loop();
}

TEST_CASE("It is safe to cancel requests in flight") {

    //
    // The general idea of this test is to measure the typical RTT with
    // respect to a server and then systematically unschedule pending DNS
    // requests when they are due, to trigger a race between receiving
    // the response and unscheduling the request.
    //
    // This regress test only repeats the process 16 times but I have
    // privately run this test repeating it for about one minute.
    //

    auto reso = ight::DNSResolver("8.8.8.8", "1");

    // Step #1: estimate the average RTT

    auto total = 0.0;
    auto count = 0;
    for (auto i = 0; i < 16; ++i) {
        auto r = reso.request("A", "www.neubot.org", [&](
                              ight::DNSResponse&& response) {
            // Assuming all the fields are OK
            total += response.get_rtt();
            count += 1;
            ight_break_loop();
        });
        ight_loop();
    }
    auto avgrtt = total / count;

    // Step #2: attempt to unschedule responses when they are due

    //for (;;) {  // only try this at home
    for (auto i = 0; i < 16; ++i) {
        auto r = new ight::DNSRequest("A", "www.neubot.org", [&](
                                      ight::DNSResponse&& /*response*/) {
            // Ignoring all the fields here
            ight_warn("- break_loop");
            ight_break_loop();
        }, reso.get_evdns_base());
        auto d = IghtDelayedCall(avgrtt, [&](void) {
            ight_warn("- cancel");
            r->cancel();
            ight_break_loop();
        });
        ight_loop();
        delete r;
    }
}

//
// The following is useful to test with tcpdump and/or nc that the
// resolver is actually sending messages to the specified address
// and port and that the desired number of retries it tried by it.
//
// It is currently commented out because I don't know how this
// test could be fully automated (especially in travis-ci).
//
// Anyway, it worked for me!
//

/*
TEST_CASE("Make sure we can override host and number of tries") {
    auto reso = ight::DNSResolver("127.0.0.1:5353", "2");
    auto r = reso.request("A", "www.neubot.org", [&](
                          ight::DNSResponse response) {
        // Assuming all the other fields are OK
        REQUIRE(response.get_results().size() == 0);
        REQUIRE(response.get_evdns_status() == DNS_ERR_TIMEOUT);
        ight_break_loop();
    });
    ight_loop();
}
*/

TEST_CASE("Evdns errors are correctly mapped to OONI failures") {
    REQUIRE(ight::DNSResponse::map_failure_(DNS_ERR_NONE)
            == "");
    REQUIRE(ight::DNSResponse::map_failure_(DNS_ERR_FORMAT)
            == "dns_lookup_error");
    REQUIRE(ight::DNSResponse::map_failure_(DNS_ERR_SERVERFAILED)
            == "dns_lookup_error");
    REQUIRE(ight::DNSResponse::map_failure_(DNS_ERR_NOTEXIST)
            == "dns_lookup_error");
    REQUIRE(ight::DNSResponse::map_failure_(DNS_ERR_NOTIMPL)
            == "dns_lookup_error");
    REQUIRE(ight::DNSResponse::map_failure_(DNS_ERR_REFUSED)
            == "dns_lookup_error");
    REQUIRE(ight::DNSResponse::map_failure_(DNS_ERR_TRUNCATED)
            == "dns_lookup_error");
    REQUIRE(ight::DNSResponse::map_failure_(DNS_ERR_UNKNOWN)
            == "unknown failure 66");
    REQUIRE(ight::DNSResponse::map_failure_(DNS_ERR_TIMEOUT)
            == "deferred_timeout_error");
    REQUIRE(ight::DNSResponse::map_failure_(DNS_ERR_SHUTDOWN)
            == "unknown failure 68");
    REQUIRE(ight::DNSResponse::map_failure_(DNS_ERR_CANCEL)
            == "unknown failure 69");
    REQUIRE(ight::DNSResponse::map_failure_(DNS_ERR_NODATA)
            == "dns_lookup_error");

    // Just three random numbers to increase confidence...
    REQUIRE(ight::DNSResponse::map_failure_(1024)
            == "unknown failure 1024");
    REQUIRE(ight::DNSResponse::map_failure_(1025)
            == "unknown failure 1025");
    REQUIRE(ight::DNSResponse::map_failure_(1026)
            == "unknown failure 1026");
}

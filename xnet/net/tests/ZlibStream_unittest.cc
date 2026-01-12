#include "xnet/net/ZlibStream.h"

#include "xnet/base/Logging.h"

#define BOOST_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include <stdio.h>

BOOST_AUTO_TEST_CASE(testZlibOutputStream)
{
    xnet::net::Buffer output;
    {
        xnet::net::ZlibOutputStream stream(&output);
        BOOST_CHECK_EQUAL(output.readableBytes(), 0);
    }
    BOOST_CHECK_EQUAL(output.readableBytes(), 8);
}

BOOST_AUTO_TEST_CASE(testZlibOutputStream1)
{
    xnet::net::Buffer output;
    xnet::net::ZlibOutputStream stream(&output);
    BOOST_CHECK_EQUAL(stream.zlibErrorCode(), Z_OK);
    stream.finish();
    BOOST_CHECK_EQUAL(stream.zlibErrorCode(), Z_STREAM_END);
}

BOOST_AUTO_TEST_CASE(testZlibOutputStream2)
{
    xnet::net::Buffer output;
    xnet::net::ZlibOutputStream stream(&output);
    BOOST_CHECK_EQUAL(stream.zlibErrorCode(), Z_OK);
    BOOST_CHECK(stream.write("01234567890123456789012345678901234567890123456789"));
    stream.finish();
    // printf("%zd\n", output.readableBytes());
    BOOST_CHECK_EQUAL(stream.zlibErrorCode(), Z_STREAM_END);
}

BOOST_AUTO_TEST_CASE(testZlibOutputStream3)
{
    xnet::net::Buffer output;
    xnet::net::ZlibOutputStream stream(&output);
    BOOST_CHECK_EQUAL(stream.zlibErrorCode(), Z_OK);
    for (int i = 0; i < 1024 * 1024; ++i)
    {
        BOOST_CHECK(stream.write("01234567890123456789012345678901234567890123456789"));
    }
    stream.finish();
    // printf("total %zd\n", output.readableBytes());
    BOOST_CHECK_EQUAL(stream.zlibErrorCode(), Z_STREAM_END);
}

BOOST_AUTO_TEST_CASE(testZlibOutputStream4)
{
    xnet::net::Buffer output;
    xnet::net::ZlibOutputStream stream(&output);
    BOOST_CHECK_EQUAL(stream.zlibErrorCode(), Z_OK);
    xnet::string input;
    for (int i = 0; i < 32768; ++i)
    {
        input += "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz_-"[rand() % 64];
    }

    for (int i = 0; i < 10; ++i)
    {
        BOOST_CHECK(stream.write(input));
    }
    stream.finish();
    // printf("total %zd\n", output.readableBytes());
    BOOST_CHECK_EQUAL(stream.zlibErrorCode(), Z_STREAM_END);
}

BOOST_AUTO_TEST_CASE(testZlibOutputStream5)
{
    xnet::net::Buffer output;
    xnet::net::ZlibOutputStream stream(&output);
    BOOST_CHECK_EQUAL(stream.zlibErrorCode(), Z_OK);
    xnet::string input(1024 * 1024, '_');
    for (int i = 0; i < 64; ++i)
    {
        BOOST_CHECK(stream.write(input));
    }
    printf("bufsiz %d\n", stream.internalOutputBufferSize());
    LOG_INFO << "total_in " << stream.inputBytes();
    LOG_INFO << "total_out " << stream.outputBytes();
    stream.finish();
    printf("total %zd\n", output.readableBytes());
    BOOST_CHECK_EQUAL(stream.zlibErrorCode(), Z_STREAM_END);
}

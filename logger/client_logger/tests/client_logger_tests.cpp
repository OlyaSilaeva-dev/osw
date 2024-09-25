#include <gtest/gtest.h>
#include "client_logger.h"
#include <iostream>

TEST(client_logger, test_console_streams)
{
    client_logger_builder builder;
    logger* new_logger = builder.add_console_stream(logger::severity::trace)
            ->add_console_stream(logger::severity::critical)
            ->add_console_stream(logger::severity::information)
            ->build();

    new_logger->information("Console information message");
    new_logger->debug("Console debug message");
    new_logger->trace("Console trace message");
    new_logger->critical("Console critical message");
}

TEST(client_logger, test_file_and_console_streams)
{
    client_logger_builder builder;
    logger* new_logger = builder.add_console_stream(logger::severity::debug)
            ->add_console_stream(logger::severity::trace)
            ->add_console_stream(logger::severity::information)
            ->add_format_string("%d" "%m")
            ->add_file_stream("C:\\logs\\test.txt", logger::severity::debug)
            ->build();

    new_logger->information("Console and file information message");
    new_logger->debug("Console and file debug message");
    new_logger->trace("Console and file trace message");
    new_logger->critical("Console and file critical message");
}

int main(int argc, char *argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

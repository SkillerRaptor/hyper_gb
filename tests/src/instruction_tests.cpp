/*
 * Copyright (c) 2025-present, SkillerRaptor
 *
 * SPDX-License-Identifier: MIT
 */

#include <filesystem>
#include <fstream>

#include <catch2/catch_test_macros.hpp>
#include <gb/cpu.h>
#include <gb/gb.h>
#include <gb/mmu.h>
#include <nlohmann/json.hpp>

struct TestState
{
    uint8_t a { 0 };
    uint8_t b { 0 };
    uint8_t c { 0 };
    uint8_t d { 0 };
    uint8_t e { 0 };
    uint8_t f { 0 };
    uint8_t h { 0 };
    uint8_t l { 0 };
    uint16_t pc { 0 };
    uint16_t sp { 0 };
    uint8_t ime { 0 };
    std::vector<std::pair<uint16_t, uint8_t>> ram;
};

void from_json(const nlohmann::json &json, TestState &test_state)
{
    json.at("a").get_to(test_state.a);
    json.at("b").get_to(test_state.b);
    json.at("c").get_to(test_state.c);
    json.at("d").get_to(test_state.d);
    json.at("e").get_to(test_state.e);
    json.at("f").get_to(test_state.f);
    json.at("h").get_to(test_state.h);
    json.at("l").get_to(test_state.l);
    json.at("pc").get_to(test_state.pc);
    json.at("sp").get_to(test_state.sp);
    json.at("ime").get_to(test_state.ime);
    json.at("ram").get_to(test_state.ram);
}

struct TestData
{
    std::string name;
    TestState initial;
    TestState final;
};

void from_json(const nlohmann::json &json, TestData &test_data)
{
    json.at("name").get_to(test_data.name);
    json.at("initial").get_to(test_data.initial);
    json.at("final").get_to(test_data.final);
}

void execute_test(const std::string &test_file)
{
    if (!std::filesystem::exists(test_file))
    {
        SKIP(test_file);
    }

    std::ifstream f(test_file);

    const nlohmann::json data = nlohmann::json::parse(f);
    const std::vector<TestData> tests = data.get<std::vector<TestData>>();

    Gb *gb = gb_create(nullptr);
    GbMmu *mmu = gb->mmu;
    GbCpu *cpu = gb->cpu;

    for (const TestData &test : tests)
    {
        cpu->registers.a = test.initial.a;
        cpu->registers.b = test.initial.b;
        cpu->registers.c = test.initial.c;
        cpu->registers.d = test.initial.d;
        cpu->registers.e = test.initial.e;
        cpu->registers.f = test.initial.f;
        cpu->registers.h = test.initial.h;
        cpu->registers.l = test.initial.l;
        cpu->registers.pc = test.initial.pc;
        cpu->registers.sp = test.initial.sp;
        cpu->interrupt_master_enable = test.initial.ime;

        for (const auto &[address, value] : test.initial.ram)
        {
            gb_mmu_write(mmu, address, value);
        }

        // Max Instruction Steps
        for (size_t i = 0; i < 100; ++i)
        {
            gb_cpu_tick(cpu);

            if (cpu->registers.pc == test.final.pc)
            {
                break;
            }
        }

        REQUIRE(cpu->registers.a == test.final.a);
        REQUIRE(cpu->registers.b == test.final.b);
        REQUIRE(cpu->registers.c == test.final.c);
        REQUIRE(cpu->registers.d == test.final.d);
        REQUIRE(cpu->registers.e == test.final.e);
        REQUIRE(cpu->registers.f == test.final.f);
        REQUIRE(cpu->registers.h == test.final.h);
        REQUIRE(cpu->registers.l == test.final.l);
        REQUIRE(cpu->registers.pc == test.final.pc);
        REQUIRE(cpu->registers.sp == test.final.sp);
        REQUIRE(cpu->interrupt_master_enable == test.final.ime);

        for (const auto &[address, value] : test.final.ram)
        {
            REQUIRE(gb_mmu_read(mmu, address) == value);
        }
    }

    gb_cpu_destroy(cpu);
    gb_mmu_destroy(mmu);
}

#define GENERATE_TEST(code) \
    TEST_CASE("Instruction 0x" code) { execute_test("./tests/data/" code ".json"); }

GENERATE_TEST("00");
GENERATE_TEST("01");
GENERATE_TEST("02");
GENERATE_TEST("03");
GENERATE_TEST("04");
GENERATE_TEST("05");
GENERATE_TEST("06");
GENERATE_TEST("07");
GENERATE_TEST("08");
GENERATE_TEST("09");
GENERATE_TEST("0a");
GENERATE_TEST("0b");
GENERATE_TEST("0c");
GENERATE_TEST("0d");
GENERATE_TEST("0e");
GENERATE_TEST("0f");

GENERATE_TEST("10");
GENERATE_TEST("11");
GENERATE_TEST("12");
GENERATE_TEST("13");
GENERATE_TEST("14");
GENERATE_TEST("15");
GENERATE_TEST("16");
GENERATE_TEST("17");
GENERATE_TEST("18");
GENERATE_TEST("19");
GENERATE_TEST("1a");
GENERATE_TEST("1b");
GENERATE_TEST("1c");
GENERATE_TEST("1d");
GENERATE_TEST("1e");
GENERATE_TEST("1f");

GENERATE_TEST("20");
GENERATE_TEST("21");
GENERATE_TEST("22");
GENERATE_TEST("23");
GENERATE_TEST("24");
GENERATE_TEST("25");
GENERATE_TEST("26");
GENERATE_TEST("27");
GENERATE_TEST("28");
GENERATE_TEST("29");
GENERATE_TEST("2a");
GENERATE_TEST("2b");
GENERATE_TEST("2c");
GENERATE_TEST("2d");
GENERATE_TEST("2e");
GENERATE_TEST("2f");

GENERATE_TEST("30");
GENERATE_TEST("31");
GENERATE_TEST("32");
GENERATE_TEST("33");
GENERATE_TEST("34");
GENERATE_TEST("35");
GENERATE_TEST("36");
GENERATE_TEST("37");
GENERATE_TEST("38");
GENERATE_TEST("39");
GENERATE_TEST("3a");
GENERATE_TEST("3b");
GENERATE_TEST("3c");
GENERATE_TEST("3d");
GENERATE_TEST("3e");
GENERATE_TEST("3f");

GENERATE_TEST("40");
GENERATE_TEST("41");
GENERATE_TEST("42");
GENERATE_TEST("43");
GENERATE_TEST("44");
GENERATE_TEST("45");
GENERATE_TEST("46");
GENERATE_TEST("47");
GENERATE_TEST("48");
GENERATE_TEST("49");
GENERATE_TEST("4a");
GENERATE_TEST("4b");
GENERATE_TEST("4c");
GENERATE_TEST("4d");
GENERATE_TEST("4e");
GENERATE_TEST("4f");

GENERATE_TEST("50");
GENERATE_TEST("51");
GENERATE_TEST("52");
GENERATE_TEST("53");
GENERATE_TEST("54");
GENERATE_TEST("55");
GENERATE_TEST("56");
GENERATE_TEST("57");
GENERATE_TEST("58");
GENERATE_TEST("59");
GENERATE_TEST("5a");
GENERATE_TEST("5b");
GENERATE_TEST("5c");
GENERATE_TEST("5d");
GENERATE_TEST("5e");
GENERATE_TEST("5f");

GENERATE_TEST("60");
GENERATE_TEST("61");
GENERATE_TEST("62");
GENERATE_TEST("63");
GENERATE_TEST("64");
GENERATE_TEST("65");
GENERATE_TEST("66");
GENERATE_TEST("67");
GENERATE_TEST("68");
GENERATE_TEST("69");
GENERATE_TEST("6a");
GENERATE_TEST("6b");
GENERATE_TEST("6c");
GENERATE_TEST("6d");
GENERATE_TEST("6e");
GENERATE_TEST("6f");

GENERATE_TEST("70");
GENERATE_TEST("71");
GENERATE_TEST("72");
GENERATE_TEST("73");
GENERATE_TEST("74");
GENERATE_TEST("75");
GENERATE_TEST("76");
GENERATE_TEST("77");
GENERATE_TEST("78");
GENERATE_TEST("79");
GENERATE_TEST("7a");
GENERATE_TEST("7b");
GENERATE_TEST("7c");
GENERATE_TEST("7d");
GENERATE_TEST("7e");
GENERATE_TEST("7f");

GENERATE_TEST("80");
GENERATE_TEST("81");
GENERATE_TEST("82");
GENERATE_TEST("83");
GENERATE_TEST("84");
GENERATE_TEST("85");
GENERATE_TEST("86");
GENERATE_TEST("87");
GENERATE_TEST("88");
GENERATE_TEST("89");
GENERATE_TEST("8a");
GENERATE_TEST("8b");
GENERATE_TEST("8c");
GENERATE_TEST("8d");
GENERATE_TEST("8e");
GENERATE_TEST("8f");

GENERATE_TEST("90");
GENERATE_TEST("91");
GENERATE_TEST("92");
GENERATE_TEST("93");
GENERATE_TEST("94");
GENERATE_TEST("95");
GENERATE_TEST("96");
GENERATE_TEST("97");
GENERATE_TEST("98");
GENERATE_TEST("99");
GENERATE_TEST("9a");
GENERATE_TEST("9b");
GENERATE_TEST("9c");
GENERATE_TEST("9d");
GENERATE_TEST("9e");
GENERATE_TEST("9f");

GENERATE_TEST("a0");
GENERATE_TEST("a1");
GENERATE_TEST("a2");
GENERATE_TEST("a3");
GENERATE_TEST("a4");
GENERATE_TEST("a5");
GENERATE_TEST("a6");
GENERATE_TEST("a7");
GENERATE_TEST("a8");
GENERATE_TEST("a9");
GENERATE_TEST("aa");
GENERATE_TEST("ab");
GENERATE_TEST("ac");
GENERATE_TEST("ad");
GENERATE_TEST("ae");
GENERATE_TEST("af");

GENERATE_TEST("b0");
GENERATE_TEST("b1");
GENERATE_TEST("b2");
GENERATE_TEST("b3");
GENERATE_TEST("b4");
GENERATE_TEST("b5");
GENERATE_TEST("b6");
GENERATE_TEST("b7");
GENERATE_TEST("b8");
GENERATE_TEST("b9");
GENERATE_TEST("ba");
GENERATE_TEST("bb");
GENERATE_TEST("bc");
GENERATE_TEST("bd");
GENERATE_TEST("be");
GENERATE_TEST("bf");

GENERATE_TEST("c0");
GENERATE_TEST("c1");
GENERATE_TEST("c2");
GENERATE_TEST("c3");
GENERATE_TEST("c4");
GENERATE_TEST("c5");
GENERATE_TEST("c6");
GENERATE_TEST("c7");
GENERATE_TEST("c8");
GENERATE_TEST("c9");
GENERATE_TEST("ca");
GENERATE_TEST("cc");
GENERATE_TEST("cd");
GENERATE_TEST("ce");
GENERATE_TEST("cf");

GENERATE_TEST("d0");
GENERATE_TEST("d1");
GENERATE_TEST("d2");
GENERATE_TEST("d4");
GENERATE_TEST("d5");
GENERATE_TEST("d6");
GENERATE_TEST("d7");
GENERATE_TEST("d8");
GENERATE_TEST("d9");
GENERATE_TEST("da");
GENERATE_TEST("dc");
GENERATE_TEST("de");
GENERATE_TEST("df");

GENERATE_TEST("e0");
GENERATE_TEST("e1");
GENERATE_TEST("e2");
GENERATE_TEST("e5");
GENERATE_TEST("e6");
GENERATE_TEST("e7");
GENERATE_TEST("e8");
GENERATE_TEST("e9");
GENERATE_TEST("ea");
GENERATE_TEST("ee");
GENERATE_TEST("ef");

GENERATE_TEST("f0");
GENERATE_TEST("f1");
GENERATE_TEST("f2");
GENERATE_TEST("f3");
GENERATE_TEST("f5");
GENERATE_TEST("f6");
GENERATE_TEST("f7");
GENERATE_TEST("f8");
GENERATE_TEST("f9");
GENERATE_TEST("fa");
GENERATE_TEST("fb");
GENERATE_TEST("fe");
GENERATE_TEST("ff");

///////////////////////

GENERATE_TEST("cb 00");
GENERATE_TEST("cb 01");
GENERATE_TEST("cb 02");
GENERATE_TEST("cb 03");
GENERATE_TEST("cb 04");
GENERATE_TEST("cb 05");
GENERATE_TEST("cb 06");
GENERATE_TEST("cb 07");
GENERATE_TEST("cb 08");
GENERATE_TEST("cb 09");
GENERATE_TEST("cb 0a");
GENERATE_TEST("cb 0b");
GENERATE_TEST("cb 0c");
GENERATE_TEST("cb 0d");
GENERATE_TEST("cb 0e");
GENERATE_TEST("cb 0f");

GENERATE_TEST("cb 10");
GENERATE_TEST("cb 11");
GENERATE_TEST("cb 12");
GENERATE_TEST("cb 13");
GENERATE_TEST("cb 14");
GENERATE_TEST("cb 15");
GENERATE_TEST("cb 16");
GENERATE_TEST("cb 17");
GENERATE_TEST("cb 18");
GENERATE_TEST("cb 19");
GENERATE_TEST("cb 1a");
GENERATE_TEST("cb 1b");
GENERATE_TEST("cb 1c");
GENERATE_TEST("cb 1d");
GENERATE_TEST("cb 1e");
GENERATE_TEST("cb 1f");

GENERATE_TEST("cb 20");
GENERATE_TEST("cb 21");
GENERATE_TEST("cb 22");
GENERATE_TEST("cb 23");
GENERATE_TEST("cb 24");
GENERATE_TEST("cb 25");
GENERATE_TEST("cb 26");
GENERATE_TEST("cb 27");
GENERATE_TEST("cb 28");
GENERATE_TEST("cb 29");
GENERATE_TEST("cb 2a");
GENERATE_TEST("cb 2b");
GENERATE_TEST("cb 2c");
GENERATE_TEST("cb 2d");
GENERATE_TEST("cb 2e");
GENERATE_TEST("cb 2f");

GENERATE_TEST("cb 30");
GENERATE_TEST("cb 31");
GENERATE_TEST("cb 32");
GENERATE_TEST("cb 33");
GENERATE_TEST("cb 34");
GENERATE_TEST("cb 35");
GENERATE_TEST("cb 36");
GENERATE_TEST("cb 37");
GENERATE_TEST("cb 38");
GENERATE_TEST("cb 39");
GENERATE_TEST("cb 3a");
GENERATE_TEST("cb 3b");
GENERATE_TEST("cb 3c");
GENERATE_TEST("cb 3d");
GENERATE_TEST("cb 3e");
GENERATE_TEST("cb 3f");

GENERATE_TEST("cb 40");
GENERATE_TEST("cb 41");
GENERATE_TEST("cb 42");
GENERATE_TEST("cb 43");
GENERATE_TEST("cb 44");
GENERATE_TEST("cb 45");
GENERATE_TEST("cb 46");
GENERATE_TEST("cb 47");
GENERATE_TEST("cb 48");
GENERATE_TEST("cb 49");
GENERATE_TEST("cb 4a");
GENERATE_TEST("cb 4b");
GENERATE_TEST("cb 4c");
GENERATE_TEST("cb 4d");
GENERATE_TEST("cb 4e");
GENERATE_TEST("cb 4f");

GENERATE_TEST("cb 50");
GENERATE_TEST("cb 51");
GENERATE_TEST("cb 52");
GENERATE_TEST("cb 53");
GENERATE_TEST("cb 54");
GENERATE_TEST("cb 55");
GENERATE_TEST("cb 56");
GENERATE_TEST("cb 57");
GENERATE_TEST("cb 58");
GENERATE_TEST("cb 59");
GENERATE_TEST("cb 5a");
GENERATE_TEST("cb 5b");
GENERATE_TEST("cb 5c");
GENERATE_TEST("cb 5d");
GENERATE_TEST("cb 5e");
GENERATE_TEST("cb 5f");

GENERATE_TEST("cb 60");
GENERATE_TEST("cb 61");
GENERATE_TEST("cb 62");
GENERATE_TEST("cb 63");
GENERATE_TEST("cb 64");
GENERATE_TEST("cb 65");
GENERATE_TEST("cb 66");
GENERATE_TEST("cb 67");
GENERATE_TEST("cb 68");
GENERATE_TEST("cb 69");
GENERATE_TEST("cb 6a");
GENERATE_TEST("cb 6b");
GENERATE_TEST("cb 6c");
GENERATE_TEST("cb 6d");
GENERATE_TEST("cb 6e");
GENERATE_TEST("cb 6f");

GENERATE_TEST("cb 70");
GENERATE_TEST("cb 71");
GENERATE_TEST("cb 72");
GENERATE_TEST("cb 73");
GENERATE_TEST("cb 74");
GENERATE_TEST("cb 75");
GENERATE_TEST("cb 76");
GENERATE_TEST("cb 77");
GENERATE_TEST("cb 78");
GENERATE_TEST("cb 79");
GENERATE_TEST("cb 7a");
GENERATE_TEST("cb 7b");
GENERATE_TEST("cb 7c");
GENERATE_TEST("cb 7d");
GENERATE_TEST("cb 7e");
GENERATE_TEST("cb 7f");

GENERATE_TEST("cb 80");
GENERATE_TEST("cb 81");
GENERATE_TEST("cb 82");
GENERATE_TEST("cb 83");
GENERATE_TEST("cb 84");
GENERATE_TEST("cb 85");
GENERATE_TEST("cb 86");
GENERATE_TEST("cb 87");
GENERATE_TEST("cb 88");
GENERATE_TEST("cb 89");
GENERATE_TEST("cb 8a");
GENERATE_TEST("cb 8b");
GENERATE_TEST("cb 8c");
GENERATE_TEST("cb 8d");
GENERATE_TEST("cb 8e");
GENERATE_TEST("cb 8f");

GENERATE_TEST("cb 90");
GENERATE_TEST("cb 91");
GENERATE_TEST("cb 92");
GENERATE_TEST("cb 93");
GENERATE_TEST("cb 94");
GENERATE_TEST("cb 95");
GENERATE_TEST("cb 96");
GENERATE_TEST("cb 97");
GENERATE_TEST("cb 98");
GENERATE_TEST("cb 99");
GENERATE_TEST("cb 9a");
GENERATE_TEST("cb 9b");
GENERATE_TEST("cb 9c");
GENERATE_TEST("cb 9d");
GENERATE_TEST("cb 9e");
GENERATE_TEST("cb 9f");

GENERATE_TEST("cb a0");
GENERATE_TEST("cb a1");
GENERATE_TEST("cb a2");
GENERATE_TEST("cb a3");
GENERATE_TEST("cb a4");
GENERATE_TEST("cb a5");
GENERATE_TEST("cb a6");
GENERATE_TEST("cb a7");
GENERATE_TEST("cb a8");
GENERATE_TEST("cb a9");
GENERATE_TEST("cb aa");
GENERATE_TEST("cb ab");
GENERATE_TEST("cb ac");
GENERATE_TEST("cb ad");
GENERATE_TEST("cb ae");
GENERATE_TEST("cb af");

GENERATE_TEST("cb b0");
GENERATE_TEST("cb b1");
GENERATE_TEST("cb b2");
GENERATE_TEST("cb b3");
GENERATE_TEST("cb b4");
GENERATE_TEST("cb b5");
GENERATE_TEST("cb b6");
GENERATE_TEST("cb b7");
GENERATE_TEST("cb b8");
GENERATE_TEST("cb b9");
GENERATE_TEST("cb ba");
GENERATE_TEST("cb bb");
GENERATE_TEST("cb bc");
GENERATE_TEST("cb bd");
GENERATE_TEST("cb be");
GENERATE_TEST("cb bf");

GENERATE_TEST("cb c0");
GENERATE_TEST("cb c1");
GENERATE_TEST("cb c2");
GENERATE_TEST("cb c3");
GENERATE_TEST("cb c4");
GENERATE_TEST("cb c5");
GENERATE_TEST("cb c6");
GENERATE_TEST("cb c7");
GENERATE_TEST("cb c8");
GENERATE_TEST("cb c9");
GENERATE_TEST("cb ca");
GENERATE_TEST("cb cb");
GENERATE_TEST("cb cc");
GENERATE_TEST("cb cd");
GENERATE_TEST("cb ce");
GENERATE_TEST("cb cf");

GENERATE_TEST("cb d0");
GENERATE_TEST("cb d1");
GENERATE_TEST("cb d2");
GENERATE_TEST("cb d3");
GENERATE_TEST("cb d4");
GENERATE_TEST("cb d5");
GENERATE_TEST("cb d6");
GENERATE_TEST("cb d7");
GENERATE_TEST("cb d8");
GENERATE_TEST("cb d9");
GENERATE_TEST("cb da");
GENERATE_TEST("cb db");
GENERATE_TEST("cb dc");
GENERATE_TEST("cb dd");
GENERATE_TEST("cb de");
GENERATE_TEST("cb df");

GENERATE_TEST("cb e0");
GENERATE_TEST("cb e1");
GENERATE_TEST("cb e2");
GENERATE_TEST("cb e3");
GENERATE_TEST("cb e4");
GENERATE_TEST("cb e5");
GENERATE_TEST("cb e6");
GENERATE_TEST("cb e7");
GENERATE_TEST("cb e8");
GENERATE_TEST("cb e9");
GENERATE_TEST("cb ea");
GENERATE_TEST("cb eb");
GENERATE_TEST("cb ec");
GENERATE_TEST("cb ed");
GENERATE_TEST("cb ee");
GENERATE_TEST("cb ef");

GENERATE_TEST("cb f0");
GENERATE_TEST("cb f1");
GENERATE_TEST("cb f2");
GENERATE_TEST("cb f3");
GENERATE_TEST("cb f4");
GENERATE_TEST("cb f5");
GENERATE_TEST("cb f6");
GENERATE_TEST("cb f7");
GENERATE_TEST("cb f8");
GENERATE_TEST("cb f9");
GENERATE_TEST("cb fa");
GENERATE_TEST("cb fb");
GENERATE_TEST("cb fc");
GENERATE_TEST("cb fd");
GENERATE_TEST("cb fe");
GENERATE_TEST("cb ff");

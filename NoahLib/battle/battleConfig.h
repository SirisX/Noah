#pragma once

struct sBattleConfig {
    void init(std::vector<u8>::iterator input) {
        m0 = READ_LE_S8(input + 0);
        m2 = READ_LE_S8(input + 2);
        for (int i = 0; i < m4.size(); i++) {
            m4[i] = READ_LE_S8(input + 4 + i);
        }
        for (int i = 0; i < m10.size(); i++) {
            m10[i] = READ_LE_S8(input + 0x10 + i);
        }
        for (int i = 0; i < m18.size(); i++) {
            m18[i] = READ_LE_S8(input + 0x18 + i);
        }
    }

    s8 m0;
    s8 m2;
    std::array<s8, 12> m4;
    std::array<s8, 8> m10;
    std::array<s8, 8> m18;
    //size 0x20
};

extern std::array<sBattleConfig, 16> battleConfigs;

extern sBattleConfig currentBattleConfig;
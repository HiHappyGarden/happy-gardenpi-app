/***************************************************************************
 *
 * Hi Happy Garden
 * Copyright (C) 2023/2024  Antonio Salsi <passy.linux@zresa.it>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 ***************************************************************************/



#pragma once

#include "hhg-iface/fsio.hpp"
#include "stm32g4xx.h"
#include "stm32g4xx_hal_flash.h"

namespace hhg::driver
{
namespace v1
{


enum class addr_flash : uint32_t
{
	//bank 1
    PAGE_0 = ((uint32_t)0x08000000), /* Base @ of Page 0, 2 Kbytes */
    PAGE_1 = ((uint32_t)0x08000800), /* Base @ of Page 1, 2 Kbytes */
    PAGE_2 = ((uint32_t)0x08001000), /* Base @ of Page 2, 2 Kbytes */
    PAGE_3 = ((uint32_t)0x08001800), /* Base @ of Page 3, 2 Kbytes */
    PAGE_4 = ((uint32_t)0x08002000), /* Base @ of Page 4, 2 Kbytes */
    PAGE_5 = ((uint32_t)0x08002800), /* Base @ of Page 5, 2 Kbytes */
    PAGE_6 = ((uint32_t)0x08003000), /* Base @ of Page 6, 2 Kbytes */
    PAGE_7 = ((uint32_t)0x08003800), /* Base @ of Page 7, 2 Kbytes */
    PAGE_8 = ((uint32_t)0x08004000), /* Base @ of Page 8, 2 Kbytes */
    PAGE_9 = ((uint32_t)0x08004800), /* Base @ of Page 9, 2 Kbytes */
    PAGE_10 = ((uint32_t)0x08005000), /* Base @ of Page 10, 2 Kbytes */
    PAGE_11 = ((uint32_t)0x08005800), /* Base @ of Page 11, 2 Kbytes */
    PAGE_12 = ((uint32_t)0x08006000), /* Base @ of Page 12, 2 Kbytes */
    PAGE_13 = ((uint32_t)0x08006800), /* Base @ of Page 13, 2 Kbytes */
    PAGE_14 = ((uint32_t)0x08007000), /* Base @ of Page 14, 2 Kbytes */
    PAGE_15 = ((uint32_t)0x08007800), /* Base @ of Page 15, 2 Kbytes */
    PAGE_16 = ((uint32_t)0x08008000), /* Base @ of Page 16, 2 Kbytes */
    PAGE_17 = ((uint32_t)0x08008800), /* Base @ of Page 17, 2 Kbytes */
    PAGE_18 = ((uint32_t)0x08009000), /* Base @ of Page 18, 2 Kbytes */
    PAGE_19 = ((uint32_t)0x08009800), /* Base @ of Page 19, 2 Kbytes */
    PAGE_20 = ((uint32_t)0x0800A000), /* Base @ of Page 20, 2 Kbytes */
    PAGE_21 = ((uint32_t)0x0800A800), /* Base @ of Page 21, 2 Kbytes */
    PAGE_22 = ((uint32_t)0x0800B000), /* Base @ of Page 22, 2 Kbytes */
    PAGE_23 = ((uint32_t)0x0800B800), /* Base @ of Page 23, 2 Kbytes */
    PAGE_24 = ((uint32_t)0x0800C000), /* Base @ of Page 24, 2 Kbytes */
    PAGE_25 = ((uint32_t)0x0800C800), /* Base @ of Page 25, 2 Kbytes */
    PAGE_26 = ((uint32_t)0x0800D000), /* Base @ of Page 26, 2 Kbytes */
    PAGE_27 = ((uint32_t)0x0800D800), /* Base @ of Page 27, 2 Kbytes */
    PAGE_28 = ((uint32_t)0x0800E000), /* Base @ of Page 28, 2 Kbytes */
    PAGE_29 = ((uint32_t)0x0800E800), /* Base @ of Page 29, 2 Kbytes */
    PAGE_30 = ((uint32_t)0x0800F000), /* Base @ of Page 30, 2 Kbytes */
    PAGE_31 = ((uint32_t)0x0800F800), /* Base @ of Page 31, 2 Kbytes */
    PAGE_32 = ((uint32_t)0x08010000), /* Base @ of Page 32, 2 Kbytes */
    PAGE_33 = ((uint32_t)0x08010800), /* Base @ of Page 33, 2 Kbytes */
    PAGE_34 = ((uint32_t)0x08011000), /* Base @ of Page 34, 2 Kbytes */
    PAGE_35 = ((uint32_t)0x08011800), /* Base @ of Page 35, 2 Kbytes */
    PAGE_36 = ((uint32_t)0x08012000), /* Base @ of Page 36, 2 Kbytes */
    PAGE_37 = ((uint32_t)0x08012800), /* Base @ of Page 37, 2 Kbytes */
    PAGE_38 = ((uint32_t)0x08013000), /* Base @ of Page 38, 2 Kbytes */
    PAGE_39 = ((uint32_t)0x08013800), /* Base @ of Page 39, 2 Kbytes */
    PAGE_40 = ((uint32_t)0x08014000), /* Base @ of Page 40, 2 Kbytes */
    PAGE_41 = ((uint32_t)0x08014800), /* Base @ of Page 41, 2 Kbytes */
    PAGE_42 = ((uint32_t)0x08015000), /* Base @ of Page 42, 2 Kbytes */
    PAGE_43 = ((uint32_t)0x08015800), /* Base @ of Page 43, 2 Kbytes */
    PAGE_44 = ((uint32_t)0x08016000), /* Base @ of Page 44, 2 Kbytes */
    PAGE_45 = ((uint32_t)0x08016800), /* Base @ of Page 45, 2 Kbytes */
    PAGE_46 = ((uint32_t)0x08017000), /* Base @ of Page 46, 2 Kbytes */
    PAGE_47 = ((uint32_t)0x08017800), /* Base @ of Page 47, 2 Kbytes */
    PAGE_48 = ((uint32_t)0x08018000), /* Base @ of Page 48, 2 Kbytes */
    PAGE_49 = ((uint32_t)0x08018800), /* Base @ of Page 49, 2 Kbytes */
    PAGE_50 = ((uint32_t)0x08019000), /* Base @ of Page 50, 2 Kbytes */
    PAGE_51 = ((uint32_t)0x08019800), /* Base @ of Page 51, 2 Kbytes */
    PAGE_52 = ((uint32_t)0x0801A000), /* Base @ of Page 52, 2 Kbytes */
    PAGE_53 = ((uint32_t)0x0801A800), /* Base @ of Page 53, 2 Kbytes */
    PAGE_54 = ((uint32_t)0x0801B000), /* Base @ of Page 54, 2 Kbytes */
    PAGE_55 = ((uint32_t)0x0801B800), /* Base @ of Page 55, 2 Kbytes */
    PAGE_56 = ((uint32_t)0x0801C000), /* Base @ of Page 56, 2 Kbytes */
    PAGE_57 = ((uint32_t)0x0801C800), /* Base @ of Page 57, 2 Kbytes */
    PAGE_58 = ((uint32_t)0x0801D000), /* Base @ of Page 58, 2 Kbytes */
    PAGE_59 = ((uint32_t)0x0801D800), /* Base @ of Page 59, 2 Kbytes */
    PAGE_60 = ((uint32_t)0x0801E000), /* Base @ of Page 60, 2 Kbytes */
    PAGE_61 = ((uint32_t)0x0801E800), /* Base @ of Page 61, 2 Kbytes */
    PAGE_62 = ((uint32_t)0x0801F000), /* Base @ of Page 62, 2 Kbytes */
    PAGE_63 = ((uint32_t)0x0801F800), /* Base @ of Page 63, 2 Kbytes */
    PAGE_64 = ((uint32_t)0x08020000), /* Base @ of Page 64, 2 Kbytes */
    PAGE_65 = ((uint32_t)0x08020800), /* Base @ of Page 65, 2 Kbytes */
    PAGE_66 = ((uint32_t)0x08021000), /* Base @ of Page 66, 2 Kbytes */
    PAGE_67 = ((uint32_t)0x08021800), /* Base @ of Page 67, 2 Kbytes */
    PAGE_68 = ((uint32_t)0x08022000), /* Base @ of Page 68, 2 Kbytes */
    PAGE_69 = ((uint32_t)0x08022800), /* Base @ of Page 69, 2 Kbytes */
    PAGE_70 = ((uint32_t)0x08023000), /* Base @ of Page 70, 2 Kbytes */
    PAGE_71 = ((uint32_t)0x08023800), /* Base @ of Page 71, 2 Kbytes */
    PAGE_72 = ((uint32_t)0x08024000), /* Base @ of Page 72, 2 Kbytes */
    PAGE_73 = ((uint32_t)0x08024800), /* Base @ of Page 73, 2 Kbytes */
    PAGE_74 = ((uint32_t)0x08025000), /* Base @ of Page 74, 2 Kbytes */
    PAGE_75 = ((uint32_t)0x08025800), /* Base @ of Page 75, 2 Kbytes */
    PAGE_76 = ((uint32_t)0x08026000), /* Base @ of Page 76, 2 Kbytes */
    PAGE_77 = ((uint32_t)0x08026800), /* Base @ of Page 77, 2 Kbytes */
    PAGE_78 = ((uint32_t)0x08027000), /* Base @ of Page 78, 2 Kbytes */
    PAGE_79 = ((uint32_t)0x08027800), /* Base @ of Page 79, 2 Kbytes */
    PAGE_80 = ((uint32_t)0x08028000), /* Base @ of Page 80, 2 Kbytes */
    PAGE_81 = ((uint32_t)0x08028800), /* Base @ of Page 81, 2 Kbytes */
    PAGE_82 = ((uint32_t)0x08029000), /* Base @ of Page 82, 2 Kbytes */
    PAGE_83 = ((uint32_t)0x08029800), /* Base @ of Page 83, 2 Kbytes */
    PAGE_84 = ((uint32_t)0x0802A000), /* Base @ of Page 84, 2 Kbytes */
    PAGE_85 = ((uint32_t)0x0802A800), /* Base @ of Page 85, 2 Kbytes */
    PAGE_86 = ((uint32_t)0x0802B000), /* Base @ of Page 86, 2 Kbytes */
    PAGE_87 = ((uint32_t)0x0802B800), /* Base @ of Page 87, 2 Kbytes */
    PAGE_88 = ((uint32_t)0x0802C000), /* Base @ of Page 88, 2 Kbytes */
    PAGE_89 = ((uint32_t)0x0802C800), /* Base @ of Page 89, 2 Kbytes */
    PAGE_90 = ((uint32_t)0x0802D000), /* Base @ of Page 90, 2 Kbytes */
    PAGE_91 = ((uint32_t)0x0802D800), /* Base @ of Page 91, 2 Kbytes */
    PAGE_92 = ((uint32_t)0x0802E000), /* Base @ of Page 92, 2 Kbytes */
    PAGE_93 = ((uint32_t)0x0802E800), /* Base @ of Page 93, 2 Kbytes */
    PAGE_94 = ((uint32_t)0x0802F000), /* Base @ of Page 94, 2 Kbytes */
    PAGE_95 = ((uint32_t)0x0802F800), /* Base @ of Page 95, 2 Kbytes */
    PAGE_96 = ((uint32_t)0x08030000), /* Base @ of Page 96, 2 Kbytes */
    PAGE_97 = ((uint32_t)0x08030800), /* Base @ of Page 97, 2 Kbytes */
    PAGE_98 = ((uint32_t)0x08031000), /* Base @ of Page 98, 2 Kbytes */
    PAGE_99 = ((uint32_t)0x08031800), /* Base @ of Page 99, 2 Kbytes */
    PAGE_100 = ((uint32_t)0x08032000), /* Base @ of Page 100, 2 Kbytes */
    PAGE_101 = ((uint32_t)0x08032800), /* Base @ of Page 101, 2 Kbytes */
    PAGE_102 = ((uint32_t)0x08033000), /* Base @ of Page 102, 2 Kbytes */
    PAGE_103 = ((uint32_t)0x08033800), /* Base @ of Page 103, 2 Kbytes */
    PAGE_104 = ((uint32_t)0x08034000), /* Base @ of Page 104, 2 Kbytes */
    PAGE_105 = ((uint32_t)0x08034800), /* Base @ of Page 105, 2 Kbytes */
    PAGE_106 = ((uint32_t)0x08035000), /* Base @ of Page 106, 2 Kbytes */
    PAGE_107 = ((uint32_t)0x08035800), /* Base @ of Page 107, 2 Kbytes */
    PAGE_108 = ((uint32_t)0x08036000), /* Base @ of Page 108, 2 Kbytes */
    PAGE_109 = ((uint32_t)0x08036800), /* Base @ of Page 109, 2 Kbytes */
    PAGE_110 = ((uint32_t)0x08037000), /* Base @ of Page 110, 2 Kbytes */
    PAGE_111 = ((uint32_t)0x08037800), /* Base @ of Page 111, 2 Kbytes */
    PAGE_112 = ((uint32_t)0x08038000), /* Base @ of Page 112, 2 Kbytes */
    PAGE_113 = ((uint32_t)0x08038800), /* Base @ of Page 113, 2 Kbytes */
    PAGE_114 = ((uint32_t)0x08039000), /* Base @ of Page 114, 2 Kbytes */
    PAGE_115 = ((uint32_t)0x08039800), /* Base @ of Page 115, 2 Kbytes */
    PAGE_116 = ((uint32_t)0x0803A000), /* Base @ of Page 116, 2 Kbytes */
    PAGE_117 = ((uint32_t)0x0803A800), /* Base @ of Page 117, 2 Kbytes */
    PAGE_118 = ((uint32_t)0x0803B000), /* Base @ of Page 118, 2 Kbytes */
    PAGE_119 = ((uint32_t)0x0803B800), /* Base @ of Page 119, 2 Kbytes */
    PAGE_120 = ((uint32_t)0x0803C000), /* Base @ of Page 120, 2 Kbytes */
    PAGE_121 = ((uint32_t)0x0803C800), /* Base @ of Page 121, 2 Kbytes */
    PAGE_122 = ((uint32_t)0x0803D000), /* Base @ of Page 122, 2 Kbytes */
    PAGE_123 = ((uint32_t)0x0803D800), /* Base @ of Page 123, 2 Kbytes */
    PAGE_124 = ((uint32_t)0x0803E000), /* Base @ of Page 124, 2 Kbytes */
    PAGE_125 = ((uint32_t)0x0803E800), /* Base @ of Page 125, 2 Kbytes */
    PAGE_126 = ((uint32_t)0x0803F000), /* Base @ of Page 126, 2 Kbytes */
    PAGE_127 = ((uint32_t)0x0803F800), /* Base @ of Page 127, 2 Kbytes */

	//bank 2
    PAGE_128 = ((uint32_t)0x08040000), /* Base @ of Page 128, 2 Kbytes */
    PAGE_129 = ((uint32_t)0x08040800), /* Base @ of Page 129, 2 Kbytes */
    PAGE_130 = ((uint32_t)0x08041000), /* Base @ of Page 130, 2 Kbytes */
    PAGE_131 = ((uint32_t)0x08041800), /* Base @ of Page 131, 2 Kbytes */
    PAGE_132 = ((uint32_t)0x08042000), /* Base @ of Page 132, 2 Kbytes */
    PAGE_133 = ((uint32_t)0x08042800), /* Base @ of Page 133, 2 Kbytes */
    PAGE_134 = ((uint32_t)0x08043000), /* Base @ of Page 134, 2 Kbytes */
    PAGE_135 = ((uint32_t)0x08043800), /* Base @ of Page 135, 2 Kbytes */
    PAGE_136 = ((uint32_t)0x08044000), /* Base @ of Page 136, 2 Kbytes */
    PAGE_137 = ((uint32_t)0x08044800), /* Base @ of Page 137, 2 Kbytes */
    PAGE_138 = ((uint32_t)0x08045000), /* Base @ of Page 138, 2 Kbytes */
    PAGE_139 = ((uint32_t)0x08045800), /* Base @ of Page 139, 2 Kbytes */
    PAGE_140 = ((uint32_t)0x08046000), /* Base @ of Page 140, 2 Kbytes */
    PAGE_141 = ((uint32_t)0x08046800), /* Base @ of Page 141, 2 Kbytes */
    PAGE_142 = ((uint32_t)0x08047000), /* Base @ of Page 142, 2 Kbytes */
    PAGE_143 = ((uint32_t)0x08047800), /* Base @ of Page 143, 2 Kbytes */
    PAGE_144 = ((uint32_t)0x08048000), /* Base @ of Page 144, 2 Kbytes */
    PAGE_145 = ((uint32_t)0x08048800), /* Base @ of Page 145, 2 Kbytes */
    PAGE_146 = ((uint32_t)0x08049000), /* Base @ of Page 146, 2 Kbytes */
    PAGE_147 = ((uint32_t)0x08049800), /* Base @ of Page 147, 2 Kbytes */
    PAGE_148 = ((uint32_t)0x0804a000), /* Base @ of Page 148, 2 Kbytes */
    PAGE_149 = ((uint32_t)0x0804a800), /* Base @ of Page 149, 2 Kbytes */
    PAGE_150 = ((uint32_t)0x0804b000), /* Base @ of Page 150, 2 Kbytes */
    PAGE_151 = ((uint32_t)0x0804b800), /* Base @ of Page 151, 2 Kbytes */
    PAGE_152 = ((uint32_t)0x0804c000), /* Base @ of Page 152, 2 Kbytes */
    PAGE_153 = ((uint32_t)0x0804c800), /* Base @ of Page 153, 2 Kbytes */
    PAGE_154 = ((uint32_t)0x0804d000), /* Base @ of Page 154, 2 Kbytes */
    PAGE_155 = ((uint32_t)0x0804d800), /* Base @ of Page 155, 2 Kbytes */
    PAGE_156 = ((uint32_t)0x0804e000), /* Base @ of Page 156, 2 Kbytes */
    PAGE_157 = ((uint32_t)0x0804e800), /* Base @ of Page 157, 2 Kbytes */
    PAGE_158 = ((uint32_t)0x0804f000), /* Base @ of Page 158, 2 Kbytes */
    PAGE_159 = ((uint32_t)0x0804f800), /* Base @ of Page 159, 2 Kbytes */
    PAGE_160 = ((uint32_t)0x08050000), /* Base @ of Page 160, 2 Kbytes */
    PAGE_161 = ((uint32_t)0x08050800), /* Base @ of Page 161, 2 Kbytes */
    PAGE_162 = ((uint32_t)0x08051000), /* Base @ of Page 162, 2 Kbytes */
    PAGE_163 = ((uint32_t)0x08051800), /* Base @ of Page 163, 2 Kbytes */
    PAGE_164 = ((uint32_t)0x08052000), /* Base @ of Page 164, 2 Kbytes */
    PAGE_165 = ((uint32_t)0x08052800), /* Base @ of Page 165, 2 Kbytes */
    PAGE_166 = ((uint32_t)0x08053000), /* Base @ of Page 166, 2 Kbytes */
    PAGE_167 = ((uint32_t)0x08053800), /* Base @ of Page 167, 2 Kbytes */
    PAGE_168 = ((uint32_t)0x08054000), /* Base @ of Page 168, 2 Kbytes */
    PAGE_169 = ((uint32_t)0x08054800), /* Base @ of Page 169, 2 Kbytes */
    PAGE_170 = ((uint32_t)0x08055000), /* Base @ of Page 170, 2 Kbytes */
    PAGE_171 = ((uint32_t)0x08055800), /* Base @ of Page 171, 2 Kbytes */
    PAGE_172 = ((uint32_t)0x08056000), /* Base @ of Page 172, 2 Kbytes */
    PAGE_173 = ((uint32_t)0x08056800), /* Base @ of Page 173, 2 Kbytes */
    PAGE_174 = ((uint32_t)0x08057000), /* Base @ of Page 174, 2 Kbytes */
    PAGE_175 = ((uint32_t)0x08057800), /* Base @ of Page 175, 2 Kbytes */
    PAGE_176 = ((uint32_t)0x08058000), /* Base @ of Page 176, 2 Kbytes */
    PAGE_177 = ((uint32_t)0x08058800), /* Base @ of Page 177, 2 Kbytes */
    PAGE_178 = ((uint32_t)0x08059000), /* Base @ of Page 178, 2 Kbytes */
    PAGE_179 = ((uint32_t)0x08059800), /* Base @ of Page 179, 2 Kbytes */
    PAGE_180 = ((uint32_t)0x0805a000), /* Base @ of Page 180, 2 Kbytes */
    PAGE_181 = ((uint32_t)0x0805a800), /* Base @ of Page 181, 2 Kbytes */
    PAGE_182 = ((uint32_t)0x0805b000), /* Base @ of Page 182, 2 Kbytes */
    PAGE_183 = ((uint32_t)0x0805b800), /* Base @ of Page 183, 2 Kbytes */
    PAGE_184 = ((uint32_t)0x0805c000), /* Base @ of Page 184, 2 Kbytes */
    PAGE_185 = ((uint32_t)0x0805c800), /* Base @ of Page 185, 2 Kbytes */
    PAGE_186 = ((uint32_t)0x0805d000), /* Base @ of Page 186, 2 Kbytes */
    PAGE_187 = ((uint32_t)0x0805d800), /* Base @ of Page 187, 2 Kbytes */
    PAGE_188 = ((uint32_t)0x0805e000), /* Base @ of Page 188, 2 Kbytes */
    PAGE_189 = ((uint32_t)0x0805e800), /* Base @ of Page 189, 2 Kbytes */
    PAGE_190 = ((uint32_t)0x0805f000), /* Base @ of Page 190, 2 Kbytes */
    PAGE_191 = ((uint32_t)0x0805f800), /* Base @ of Page 191, 2 Kbytes */
    PAGE_192 = ((uint32_t)0x08060000), /* Base @ of Page 192, 2 Kbytes */
    PAGE_193 = ((uint32_t)0x08060800), /* Base @ of Page 193, 2 Kbytes */
    PAGE_194 = ((uint32_t)0x08061000), /* Base @ of Page 194, 2 Kbytes */
    PAGE_195 = ((uint32_t)0x08061800), /* Base @ of Page 195, 2 Kbytes */
    PAGE_196 = ((uint32_t)0x08062000), /* Base @ of Page 196, 2 Kbytes */
    PAGE_197 = ((uint32_t)0x08062800), /* Base @ of Page 197, 2 Kbytes */
    PAGE_198 = ((uint32_t)0x08063000), /* Base @ of Page 198, 2 Kbytes */
    PAGE_199 = ((uint32_t)0x08063800), /* Base @ of Page 199, 2 Kbytes */
    PAGE_200 = ((uint32_t)0x08064000), /* Base @ of Page 200, 2 Kbytes */
    PAGE_201 = ((uint32_t)0x08064800), /* Base @ of Page 201, 2 Kbytes */
    PAGE_202 = ((uint32_t)0x08065000), /* Base @ of Page 202, 2 Kbytes */
    PAGE_203 = ((uint32_t)0x08065800), /* Base @ of Page 203, 2 Kbytes */
    PAGE_204 = ((uint32_t)0x08066000), /* Base @ of Page 204, 2 Kbytes */
    PAGE_205 = ((uint32_t)0x08066800), /* Base @ of Page 205, 2 Kbytes */
    PAGE_206 = ((uint32_t)0x08067000), /* Base @ of Page 206, 2 Kbytes */
    PAGE_207 = ((uint32_t)0x08067800), /* Base @ of Page 207, 2 Kbytes */
    PAGE_208 = ((uint32_t)0x08068000), /* Base @ of Page 208, 2 Kbytes */
    PAGE_209 = ((uint32_t)0x08068800), /* Base @ of Page 209, 2 Kbytes */
    PAGE_210 = ((uint32_t)0x08069000), /* Base @ of Page 210, 2 Kbytes */
    PAGE_211 = ((uint32_t)0x08069800), /* Base @ of Page 211, 2 Kbytes */
    PAGE_212 = ((uint32_t)0x0806a000), /* Base @ of Page 212, 2 Kbytes */
    PAGE_213 = ((uint32_t)0x0806a800), /* Base @ of Page 213, 2 Kbytes */
    PAGE_214 = ((uint32_t)0x0806b000), /* Base @ of Page 214, 2 Kbytes */
    PAGE_215 = ((uint32_t)0x0806b800), /* Base @ of Page 215, 2 Kbytes */
    PAGE_216 = ((uint32_t)0x0806c000), /* Base @ of Page 216, 2 Kbytes */
    PAGE_217 = ((uint32_t)0x0806c800), /* Base @ of Page 217, 2 Kbytes */
    PAGE_218 = ((uint32_t)0x0806d000), /* Base @ of Page 218, 2 Kbytes */
    PAGE_219 = ((uint32_t)0x0806d800), /* Base @ of Page 219, 2 Kbytes */
    PAGE_220 = ((uint32_t)0x0806e000), /* Base @ of Page 220, 2 Kbytes */
    PAGE_221 = ((uint32_t)0x0806e800), /* Base @ of Page 221, 2 Kbytes */
    PAGE_222 = ((uint32_t)0x0806f000), /* Base @ of Page 222, 2 Kbytes */
    PAGE_223 = ((uint32_t)0x0806f800), /* Base @ of Page 223, 2 Kbytes */
    PAGE_224 = ((uint32_t)0x08070000), /* Base @ of Page 224, 2 Kbytes */
    PAGE_225 = ((uint32_t)0x08070800), /* Base @ of Page 225, 2 Kbytes */
    PAGE_226 = ((uint32_t)0x08071000), /* Base @ of Page 226, 2 Kbytes */
    PAGE_227 = ((uint32_t)0x08071800), /* Base @ of Page 227, 2 Kbytes */
    PAGE_228 = ((uint32_t)0x08072000), /* Base @ of Page 228, 2 Kbytes */
    PAGE_229 = ((uint32_t)0x08072800), /* Base @ of Page 229, 2 Kbytes */
    PAGE_230 = ((uint32_t)0x08073000), /* Base @ of Page 230, 2 Kbytes */
    PAGE_231 = ((uint32_t)0x08073800), /* Base @ of Page 231, 2 Kbytes */
    PAGE_232 = ((uint32_t)0x08074000), /* Base @ of Page 232, 2 Kbytes */
    PAGE_233 = ((uint32_t)0x08074800), /* Base @ of Page 233, 2 Kbytes */
    PAGE_234 = ((uint32_t)0x08075000), /* Base @ of Page 234, 2 Kbytes */
    PAGE_235 = ((uint32_t)0x08075800), /* Base @ of Page 235, 2 Kbytes */
    PAGE_236 = ((uint32_t)0x08076000), /* Base @ of Page 236, 2 Kbytes */
    PAGE_237 = ((uint32_t)0x08076800), /* Base @ of Page 237, 2 Kbytes */
    PAGE_238 = ((uint32_t)0x08077000), /* Base @ of Page 238, 2 Kbytes */
    PAGE_239 = ((uint32_t)0x08077800), /* Base @ of Page 239, 2 Kbytes */
    PAGE_240 = ((uint32_t)0x08078000), /* Base @ of Page 240, 2 Kbytes */
    PAGE_241 = ((uint32_t)0x08078800), /* Base @ of Page 241, 2 Kbytes */
    PAGE_242 = ((uint32_t)0x08079000), /* Base @ of Page 242, 2 Kbytes */
    PAGE_243 = ((uint32_t)0x08079800), /* Base @ of Page 243, 2 Kbytes */
    PAGE_244 = ((uint32_t)0x0807a000), /* Base @ of Page 244, 2 Kbytes */
    PAGE_245 = ((uint32_t)0x0807a800), /* Base @ of Page 245, 2 Kbytes */
    PAGE_246 = ((uint32_t)0x0807b000), /* Base @ of Page 246, 2 Kbytes */
    PAGE_247 = ((uint32_t)0x0807b800), /* Base @ of Page 247, 2 Kbytes */
    PAGE_248 = ((uint32_t)0x0807c000), /* Base @ of Page 248, 2 Kbytes */
    PAGE_249 = ((uint32_t)0x0807c800), /* Base @ of Page 249, 2 Kbytes */
    PAGE_250 = ((uint32_t)0x0807d000), /* Base @ of Page 250, 2 Kbytes */
    PAGE_251 = ((uint32_t)0x0807d800), /* Base @ of Page 251, 2 Kbytes */
    PAGE_252 = ((uint32_t)0x0807e000), /* Base @ of Page 252, 2 Kbytes */
    PAGE_253 = ((uint32_t)0x0807e800), /* Base @ of Page 253, 2 Kbytes */
    PAGE_254 = ((uint32_t)0x0807f000), /* Base @ of Page 254, 2 Kbytes */
    PAGE_255 = ((uint32_t)0x0807f800), /* Base @ of Page 255, 2 Kbytes */
};



class stm32_fsio final : public iface::v1::fsio
{
	uint32_t start_flash_address = 0;
	uint32_t end_flash_address = 0;

	/*Variable used to handle the Options Bytes*/
	mutable FLASH_OBProgramInitTypeDef options_bytes_struct1;
	mutable FLASH_OBProgramInitTypeDef options_bytes_struct2;
public:

	enum max_size
	{
		CONFIG = 5 * FLASH_PAGE_SIZE,
		DATA = 10 * FLASH_PAGE_SIZE
	};

	static uint64_t const check_data;

	stm32_fsio(uint32_t start_flash_address, uint32_t end_flash_address) OS_NOEXCEPT;
	virtual ~stm32_fsio();

	os::exit init(os::error** error) OS_NOEXCEPT override;

	os::exit write(iface::v1::data_type type, const uint8_t data[], size_t size, os::error** error) const OS_NOEXCEPT override;

	os::exit read(iface::v1::data_type type, uint8_t data[], size_t size, os::error** error) const OS_NOEXCEPT override;

private:
	struct meta_address
	{
		uint32_t address = 0;
		uint32_t bank = FLASH_BANK_1;
	};

	static uint32_t get_page(uint32_t address) OS_NOEXCEPT;
	os::pair<meta_address, meta_address> get_start_and_end_addresses(iface::v1::data_type type, size_t size, os::error** error) const OS_NOEXCEPT;
};

}
}


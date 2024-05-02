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

#include "hhg-iface/initializable.hpp"

namespace hhg::iface
{
inline namespace v1
{

struct lcd : public initializable
{
    using ptr = os::unique_ptr<hhg::iface::lcd>;

    struct io
    {
        io() = default;
        virtual ~io() = default;
        OS_NO_COPY_NO_MOVE(io)
        virtual os::exit init(os::error** error) const OS_NOEXCEPT = 0;
        virtual int32_t write(const uint8_t*, size_t data_len) const OS_NOEXCEPT = 0;
        virtual int32_t read(uint8_t*, size_t data_size) const OS_NOEXCEPT = 0;
        virtual void us_sleep(uint64_t us) const OS_NOEXCEPT = 0;
    };

    /// \enum pico_sh1106::WriteMode
    enum class write_mode : const uint8_t
    {
        /// sets pixel on regardless of its state
        ADD = 0,
                /// sets pixel off regardless of its state
        SUBTRACT = 1,
                /// inverts pixel, so 1->0 or 0->1
        INVERT = 2,
    };

    mutable uint8_t back_light = 0;

    virtual ~lcd() OS_NOEXCEPT = default;

    virtual void set_pixel(int16_t x, int16_t y, write_mode mode = write_mode::ADD) const OS_NOEXCEPT = 0;

    /// \brief Sends frame buffer to display so that it updated
    virtual void send_buffer() OS_NOEXCEPT = 0;

    /// \brief Adds bitmap image to frame buffer
    /// \param anchorX - sets start point of where to put the image on the screen
    /// \param anchorY - sets start point of where to put the image on the screen
    /// \param image_width - width of the image in pixels
    /// \param image_height - height of the image in pixels
    /// \param image - pointer to uint8_t (unsigned char) array containing image data
    /// \param mode - mode describes setting behavior. See WriteMode doc for more information
    virtual void add_bitmap_image(int16_t anchor_x, int16_t anchor_y, uint8_t image_width, uint8_t image_height, uint8_t *image, write_mode mode = write_mode::ADD) OS_NOEXCEPT = 0;

    /// \brief Manually set frame buffer. make sure it's correct size of 1024 bytes
    /// \param buffer - pointer to a new buffer
    virtual void set_buffer(uint8_t *buffer, size_t buffer_size) OS_NOEXCEPT = 0;

    /// \brief Flips the display
    /// \param orientation - 0 for not flipped, 1 for flipped display
    virtual void set_orientation(bool orientation) OS_NOEXCEPT = 0;


    /// \brief Clears frame buffer aka set all bytes to 0
    virtual void clear() OS_NOEXCEPT = 0;

    /// \brief Inverts screen on hardware level. Way more efficient than setting buffer to all ones and then using WriteMode subtract.
    virtual void invert_display() OS_NOEXCEPT = 0;

    /// \brief Sets display contrast according to sh1106 documentation
    /// \param contrast - accepted values of 0 to 255 to set the contrast
    virtual void set_contrast(uint8_t contrast) OS_NOEXCEPT = 0;

    /// \brief Turns display off
    virtual void turn_off() const OS_NOEXCEPT = 0;

    /// \brief Turns display on
    virtual void turn_on() const OS_NOEXCEPT = 0;
};

} // hhg
}
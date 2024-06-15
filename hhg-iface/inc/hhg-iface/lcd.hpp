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

    /// \enum pico_sh1106::WriteMode
    enum class write_mode : const uint8_t
    {
        /// sets pixel on regardless of its state
        ADD = 0,
                /// sets pixel off regardless of its state
        REMOVE = 1,
                /// inverts pixel, so 1->0 or 0->1
        INVERT = 2,
    };

    ~lcd() OSAL_NOEXCEPT override = default;

    virtual void set_pixel(uint16_t x, uint16_t y, write_mode mode) const OSAL_NOEXCEPT = 0;

    virtual void set_pixel(uint16_t x, uint16_t y) const OSAL_NOEXCEPT = 0;

    /// \brief Sends frame buffer to display so that it updated
    virtual void send_buffer() OSAL_NOEXCEPT = 0;

    /// \brief Adds bitmap image to frame buffer
    /// \param anchorX - sets start point of where to put the image on the screen
    /// \param anchorY - sets start point of where to put the image on the screen
    /// \param width - width of the image in pixels
    /// \param height - height of the image in pixels
    /// \param image - pointer to uint8_t (unsigned char) array containing image data
    /// \param mode - mode describes setting behavior. See WriteMode doc for more information
    virtual void set_bitmap_image(uint16_t x, uint16_t y, uint16_t width, uint16_t height, const uint8_t *image, uint32_t image_size) OSAL_NOEXCEPT = 0;

    virtual void set_rect(uint16_t x, uint16_t y, uint16_t width, uint16_t height, write_mode mode) OSAL_NOEXCEPT = 0;

    virtual void set_char(char c, uint16_t x, uint16_t y, const uint8_t* font, uint32_t font_size) OSAL_NOEXCEPT = 0;

    virtual void set_str(const char str[], uint16_t x, uint16_t y, const uint8_t* font, uint32_t font_size) OSAL_NOEXCEPT = 0;

    virtual os::pair<uint16_t, uint16_t> get_size() const OSAL_NOEXCEPT = 0;

    /// \brief Manually set frame buffer. make sure it's correct size of 1024 bytes
    /// \param buffer - pointer to a new buffer
    virtual void set_buffer(uint8_t *buffer, size_t buffer_size) OSAL_NOEXCEPT = 0;

    /// \brief Flips the display
    /// \param orientation - 0 for not flipped, 1 for flipped display
    virtual void invert_orientation() OSAL_NOEXCEPT = 0;


    /// \brief Clears frame buffer aka set all bytes to 0
    virtual void clear() OSAL_NOEXCEPT = 0;


    /// \brief Sets display contrast according to sh1106 documentation
    /// \param contrast - accepted values of 0 to 255 to set the contrast
    virtual void set_contrast(uint8_t contrast) OSAL_NOEXCEPT = 0;

    /// \brief Turns display off
    virtual void turn_off() const OSAL_NOEXCEPT = 0;

    /// \brief Turns display on
    virtual void turn_on() const OSAL_NOEXCEPT = 0;


};

} // hhg
}
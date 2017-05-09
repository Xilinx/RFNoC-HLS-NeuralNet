/* -*- c++ -*- */
/* 
 * Copyright 2017 <+YOU OR YOUR COMPANY+>.
 * 
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 * 
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */


#include <fpgannet/ex1layer_block_ctrl.hpp>
#include <uhd/convert.hpp>
#include <uhd/utils/msg.hpp>

using namespace uhd::rfnoc;

class ex1layer_block_ctrl_impl : public ex1layer_block_ctrl
{
public:

    UHD_RFNOC_BLOCK_CONSTRUCTOR(ex1layer_block_ctrl)
    {

    }
private:

};

UHD_RFNOC_BLOCK_REGISTER(ex1layer_block_ctrl,"ex1layer");

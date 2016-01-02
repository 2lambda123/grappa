////////////////////////////////////////////////////////////////////////
// Copyright (c) 2010-2015, University of Washington and Battelle
// Memorial Institute.  All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
//     * Redistributions of source code must retain the above
//       copyright notice, this list of conditions and the following
//       disclaimer.
//     * Redistributions in binary form must reproduce the above
//       copyright notice, this list of conditions and the following
//       disclaimer in the documentation and/or other materials
//       provided with the distribution.
//     * Neither the name of the University of Washington, Battelle
//       Memorial Institute, or the names of their contributors may be
//       used to endorse or promote products derived from this
//       software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
// FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
// UNIVERSITY OF WASHINGTON OR BATTELLE MEMORIAL INSTITUTE BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
// OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
// BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
// LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
// USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
// DAMAGE.
////////////////////////////////////////////////////////////////////////

#include <RMA.hpp>

namespace Grappa {
namespace impl {

RMA global_rma;

// specialize for void *
template<>
RMAAddress<void> RMA::to_global<void>( Core core, void * local ) {
  auto local_int = reinterpret_cast< intptr_t >( local );
  auto it = get_enclosing( core, local_int );
  CHECK( it != address_maps_[core].end() ) << "No mapping found for " << (void*) local << " on core " << core;
  auto byte_offset = local_int - it->first;
  CHECK_LT( byte_offset, std::numeric_limits<MPI_Aint>::max() ) << "Operation would overflow MPI offset argument type";
  return RMAAddress<void>( static_cast<void*>( it->second.base_ ),
                           it->second.window_,
                           byte_offset );
}

// specialize for const void *
template<>
RMAAddress<const void> RMA::to_global<const void>( Core core, const void * local ) {
  auto local_int = reinterpret_cast< intptr_t >( local );
  auto it = get_enclosing( core, local_int );
  CHECK( it != address_maps_[core].end() ) << "No mapping found for " << (void*) local << " on core " << core;
  auto byte_offset = local_int - it->first;
  CHECK_LT( byte_offset, std::numeric_limits<MPI_Aint>::max() ) << "Operation would overflow MPI offset argument type";
  return RMAAddress<const void>( static_cast<void*>( it->second.base_ ),
                                 it->second.window_,
                                 byte_offset );
}

} // namespace impl
} // namespace Grappa


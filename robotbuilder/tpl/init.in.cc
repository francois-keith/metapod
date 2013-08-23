@LICENSE@
// This file has been generated by the metapod robotbuilder library.

#ifdef _MSC_VER
# pragma warning( push )
// The following warnings are only needed if the FloatType is float, because
// the code generator uses double anyway.
// disable warning C4305 truncation from 'double' to 'float'
// disable warning C4244 conversion from 'double' to 'float', possible loss of data
# pragma warning( disable: 4305 4244)
#endif

#include "@ROBOT_NAME@.hh"

namespace metapod {

@init_nodes@

template <> Spatial::InertiaTpl<FloatType> @ROBOT_CLASS_NAME@<FloatType>::inertias[@node_id@] = {
@init_inertias@};
}

#ifdef _MSC_VER
# pragma warning( pop )
#endif

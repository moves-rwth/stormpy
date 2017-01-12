#include <boost/optional.hpp>

namespace pybind11 {
    namespace detail {
template<typename T> struct boost_optional_caster {
    using value_conv = make_caster<typename T::value_type>;

    static handle cast(const T& src, return_value_policy policy, handle parent) {
        if (!src)
            return none().inc_ref();
        return value_conv::cast(*src, policy, parent);
    }

    bool load(handle src, bool convert) {
        if (!src) {
            return false;
        } else if (src.is_none()) {
            value = boost::none;  // nullopt
            return true;
        }
        value_conv inner_caster;
        if (!inner_caster.load(src, convert))
            return false;

        value.emplace(cast_op<typename T::value_type>(inner_caster));
        return true;
    }

    PYBIND11_TYPE_CASTER(T, _("Optional[") + value_conv::name() + _("]"));
};

template<typename T> struct type_caster<boost::optional<T>>
    : public boost_optional_caster<boost::optional<T>> {};

template<> struct type_caster<boost::none_t>
    : public void_caster<boost::none_t> {};

    }
}

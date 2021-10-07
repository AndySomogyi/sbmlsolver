
%typemap(out) const rr::Variant& {
    try {
        const rr::Variant& temp = *($1);
        $result = Variant_to_py(temp);
    } catch (const std::exception& e) {
        SWIG_exception(SWIG_RuntimeError, e.what());
    }
}


%typemap(out) const rr::Variant {
    try {
        $result = Variant_to_py($1);
    } catch (const std::exception& e) {
        SWIG_exception(SWIG_RuntimeError, e.what());
    }
}

%apply const rr::Variant {Variant, rr::Variant, const Variant};


%typemap(in) const rr::Variant& (rr::Variant temp) {

    try {
        temp = Variant_from_py($input);
        $1 = &temp;
    } catch (const std::exception& e) {
        SWIG_exception(SWIG_RuntimeError, e.what());
    }
}

%apply const rr::Variant& {rr::Variant&, Variant&, const Variant&};

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <functional>
#include <sstream>  // __str__
#include <ssw_cpp20.hpp>
#include <string>

#ifndef BINDER_PYBIND11_TYPE_CASTER
#  define BINDER_PYBIND11_TYPE_CASTER
PYBIND11_DECLARE_HOLDER_TYPE(T, std::shared_ptr<T>)
PYBIND11_DECLARE_HOLDER_TYPE(T, T *)
PYBIND11_MAKE_OPAQUE(std::shared_ptr<void>)
#endif

// clang-format off
void bind_ssw_cpp20(std::function< pybind11::module &(std::string const &namespace_) > &M)
{
	{ // StripedSmithWaterman::Alignment file:ssw_cpp20.hpp line:15
		pybind11::class_<StripedSmithWaterman::Alignment, std::shared_ptr<StripedSmithWaterman::Alignment>> cl(M("StripedSmithWaterman"), "Alignment", "" );
		cl.def( pybind11::init( [](){ return new StripedSmithWaterman::Alignment(); } ) );
		cl.def( pybind11::init( [](StripedSmithWaterman::Alignment const &o){ return new StripedSmithWaterman::Alignment(o); } ) );
		cl.def_readwrite("sw_score", &StripedSmithWaterman::Alignment::sw_score);
		cl.def_readwrite("sw_score_next_best", &StripedSmithWaterman::Alignment::sw_score_next_best);
		cl.def_readwrite("ref_begin", &StripedSmithWaterman::Alignment::ref_begin);
		cl.def_readwrite("ref_end", &StripedSmithWaterman::Alignment::ref_end);
		cl.def_readwrite("query_begin", &StripedSmithWaterman::Alignment::query_begin);
		cl.def_readwrite("query_end", &StripedSmithWaterman::Alignment::query_end);
		cl.def_readwrite("ref_end_next_best", &StripedSmithWaterman::Alignment::ref_end_next_best);
		cl.def_readwrite("mismatches", &StripedSmithWaterman::Alignment::mismatches);
		cl.def_readwrite("cigar_string", &StripedSmithWaterman::Alignment::cigar_string);
		cl.def_readwrite("cigar", &StripedSmithWaterman::Alignment::cigar);
		cl.def("Clear", (void (StripedSmithWaterman::Alignment::*)()) &StripedSmithWaterman::Alignment::Clear, "C++: StripedSmithWaterman::Alignment::Clear() --> void");
//		cl.def("assign", (struct StripedSmithWaterman::Alignment & (StripedSmithWaterman::Alignment::*)(const struct StripedSmithWaterman::Alignment &)) &StripedSmithWaterman::Alignment::operator=, "C++: StripedSmithWaterman::Alignment::operator=(const struct StripedSmithWaterman::Alignment &) --> struct StripedSmithWaterman::Alignment &", pybind11::return_value_policy::automatic, pybind11::arg(""));
	}
	{ // StripedSmithWaterman::Filter file:ssw_cpp20.hpp line:42
		pybind11::class_<StripedSmithWaterman::Filter, std::shared_ptr<StripedSmithWaterman::Filter>> cl(M("StripedSmithWaterman"), "Filter", "" );
		cl.def( pybind11::init( [](){ return new StripedSmithWaterman::Filter(); } ) );
		cl.def( pybind11::init<bool, bool, unsigned short, unsigned short>(), pybind11::arg("pos"), pybind11::arg("cigar"), pybind11::arg("score"), pybind11::arg("dis") );

		cl.def_readwrite("report_begin_position", &StripedSmithWaterman::Filter::report_begin_position);
		cl.def_readwrite("report_cigar", &StripedSmithWaterman::Filter::report_cigar);
		cl.def_readwrite("score_filter", &StripedSmithWaterman::Filter::score_filter);
		cl.def_readwrite("distance_filter", &StripedSmithWaterman::Filter::distance_filter);
	}
	{ // StripedSmithWaterman::Aligner file:ssw_cpp20.hpp line:68
		pybind11::class_<StripedSmithWaterman::Aligner, std::shared_ptr<StripedSmithWaterman::Aligner>> cl(M("StripedSmithWaterman"), "Aligner", "" );
		cl.def( pybind11::init( [](){ return new StripedSmithWaterman::Aligner(); } ) );
		cl.def( pybind11::init<unsigned char, unsigned char, unsigned char, unsigned char>(), pybind11::arg("match_score"), pybind11::arg("mismatch_penalty"), pybind11::arg("gap_opening_penalty"), pybind11::arg("gap_extending_penalty") );

		cl.def( pybind11::init<const signed char *, int, const signed char *, int>(), pybind11::arg("score_matrix"), pybind11::arg("score_matrix_size"), pybind11::arg("translation_matrix"), pybind11::arg("translation_matrix_size") );

		cl.def("SetReferenceSequence", (int (StripedSmithWaterman::Aligner::*)(const char *, int)) &StripedSmithWaterman::Aligner::SetReferenceSequence, "C++: StripedSmithWaterman::Aligner::SetReferenceSequence(const char *, int) --> int", pybind11::arg("seq"), pybind11::arg("length"));
		cl.def("CleanReferenceSequence", (void (StripedSmithWaterman::Aligner::*)()) &StripedSmithWaterman::Aligner::CleanReferenceSequence, "C++: StripedSmithWaterman::Aligner::CleanReferenceSequence() --> void");
		cl.def("SetGapPenalty", (void (StripedSmithWaterman::Aligner::*)(unsigned char, unsigned char)) &StripedSmithWaterman::Aligner::SetGapPenalty, "C++: StripedSmithWaterman::Aligner::SetGapPenalty(unsigned char, unsigned char) --> void", pybind11::arg("opening"), pybind11::arg("extending"));
		cl.def("Align", (bool (StripedSmithWaterman::Aligner::*)(const char *, const struct StripedSmithWaterman::Filter &, struct StripedSmithWaterman::Alignment *, int) const) &StripedSmithWaterman::Aligner::Align, "C++: StripedSmithWaterman::Aligner::Align(const char *, const struct StripedSmithWaterman::Filter &, struct StripedSmithWaterman::Alignment *, int) const --> bool", pybind11::arg("query"), pybind11::arg("filter"), pybind11::arg("alignment"), pybind11::arg("maskLen"));
		cl.def("Align", (bool (StripedSmithWaterman::Aligner::*)(const char *, const char *, int, const struct StripedSmithWaterman::Filter &, struct StripedSmithWaterman::Alignment *, int) const) &StripedSmithWaterman::Aligner::Align, "C++: StripedSmithWaterman::Aligner::Align(const char *, const char *, int, const struct StripedSmithWaterman::Filter &, struct StripedSmithWaterman::Alignment *, int) const --> bool", pybind11::arg("query"), pybind11::arg("ref"), pybind11::arg("ref_len"), pybind11::arg("filter"), pybind11::arg("alignment"), pybind11::arg("maskLen"));
		cl.def("Clear", (void (StripedSmithWaterman::Aligner::*)()) &StripedSmithWaterman::Aligner::Clear, "C++: StripedSmithWaterman::Aligner::Clear() --> void");
		cl.def("ReBuild", (bool (StripedSmithWaterman::Aligner::*)()) &StripedSmithWaterman::Aligner::ReBuild, "C++: StripedSmithWaterman::Aligner::ReBuild() --> bool");
		cl.def("ReBuild", (bool (StripedSmithWaterman::Aligner::*)(unsigned char, unsigned char, unsigned char, unsigned char)) &StripedSmithWaterman::Aligner::ReBuild, "C++: StripedSmithWaterman::Aligner::ReBuild(unsigned char, unsigned char, unsigned char, unsigned char) --> bool", pybind11::arg("match_score"), pybind11::arg("mismatch_penalty"), pybind11::arg("gap_opening_penalty"), pybind11::arg("gap_extending_penalty"));
		cl.def("ReBuild", (bool (StripedSmithWaterman::Aligner::*)(const signed char *, int, const signed char *, int)) &StripedSmithWaterman::Aligner::ReBuild, "C++: StripedSmithWaterman::Aligner::ReBuild(const signed char *, int, const signed char *, int) --> bool", pybind11::arg("score_matrix"), pybind11::arg("score_matrix_size"), pybind11::arg("translation_matrix"), pybind11::arg("translation_matrix_size"));
	}
}

// clang-format on

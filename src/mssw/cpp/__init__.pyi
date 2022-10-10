"""Bindings for ::StripedSmithWaterman namespace"""
import mssw._cpp.StripedSmithWaterman # type: ignore
import typing

__all__ = [
    "Aligner",
    "Alignment",
    "Filter"
]


class Aligner:
    @typing.overload
    def Align(self, query: str, filter: Filter, alignment: Alignment, maskLen: int) -> bool:
        """
        C++: StripedSmithWaterman::Aligner::Align(const char *, const struct StripedSmithWaterman::Filter &, struct StripedSmithWaterman::Alignment *, int) const --> bool

        C++: StripedSmithWaterman::Aligner::Align(const char *, const char *, int, const struct StripedSmithWaterman::Filter &, struct StripedSmithWaterman::Alignment *, int) const --> bool
        """

    @typing.overload
    def Align(self, query: str, ref: str, ref_len: int, filter: Filter, alignment: Alignment, maskLen: int) -> bool: ...

    def CleanReferenceSequence(self) -> None:
        """
        C++: StripedSmithWaterman::Aligner::CleanReferenceSequence() --> void
        """

    def Clear(self) -> None:
        """
        C++: StripedSmithWaterman::Aligner::Clear() --> void
        """

    @typing.overload
    def ReBuild(self) -> bool:
        """
        C++: StripedSmithWaterman::Aligner::ReBuild() --> bool

        C++: StripedSmithWaterman::Aligner::ReBuild(unsigned char, unsigned char, unsigned char, unsigned char) --> bool

        C++: StripedSmithWaterman::Aligner::ReBuild(const signed char *, int, const signed char *, int) --> bool
        """

    @typing.overload
    def ReBuild(self, match_score: int, mismatch_penalty: int, gap_opening_penalty: int,
                gap_extending_penalty: int) -> bool: ...

    @typing.overload
    def ReBuild(self, score_matrix: int, score_matrix_size: int, translation_matrix: int,
                translation_matrix_size: int) -> bool: ...

    def SetGapPenalty(self, opening: int, extending: int) -> None:
        """
        C++: StripedSmithWaterman::Aligner::SetGapPenalty(unsigned char, unsigned char) --> void
        """

    def SetReferenceSequence(self, seq: str, length: int) -> int:
        """
        C++: StripedSmithWaterman::Aligner::SetReferenceSequence(const char *, int) --> int
        """

    @typing.overload
    def __init__(self) -> None: ...

    @typing.overload
    def __init__(self, match_score: int, mismatch_penalty: int, gap_opening_penalty: int,
                 gap_extending_penalty: int) -> None: ...

    @typing.overload
    def __init__(self, score_matrix: int, score_matrix_size: int, translation_matrix: int,
                 translation_matrix_size: int) -> None: ...

    pass


class Alignment:
    def Clear(self) -> None:
        """
        C++: StripedSmithWaterman::Alignment::Clear() --> void
        """

    @typing.overload
    def __init__(self) -> None: ...

    @typing.overload
    def __init__(self, arg0: Alignment) -> None: ...

    @property
    def cigar(self) -> typing.List[int]:
        """
        :type: typing.List[int]
        """

    @cigar.setter
    def cigar(self, arg0: typing.List[int]) -> None:
        pass

    @property
    def cigar_string(self) -> str:
        """
        :type: str
        """

    @cigar_string.setter
    def cigar_string(self, arg0: str) -> None:
        pass

    @property
    def mismatches(self) -> int:
        """
        :type: int
        """

    @mismatches.setter
    def mismatches(self, arg0: int) -> None:
        pass

    @property
    def query_begin(self) -> int:
        """
        :type: int
        """

    @query_begin.setter
    def query_begin(self, arg0: int) -> None:
        pass

    @property
    def query_end(self) -> int:
        """
        :type: int
        """

    @query_end.setter
    def query_end(self, arg0: int) -> None:
        pass

    @property
    def ref_begin(self) -> int:
        """
        :type: int
        """

    @ref_begin.setter
    def ref_begin(self, arg0: int) -> None:
        pass

    @property
    def ref_end(self) -> int:
        """
        :type: int
        """

    @ref_end.setter
    def ref_end(self, arg0: int) -> None:
        pass

    @property
    def ref_end_next_best(self) -> int:
        """
        :type: int
        """

    @ref_end_next_best.setter
    def ref_end_next_best(self, arg0: int) -> None:
        pass

    @property
    def sw_score(self) -> int:
        """
        :type: int
        """

    @sw_score.setter
    def sw_score(self, arg0: int) -> None:
        pass

    @property
    def sw_score_next_best(self) -> int:
        """
        :type: int
        """

    @sw_score_next_best.setter
    def sw_score_next_best(self, arg0: int) -> None:
        pass

    pass


class Filter:
    @typing.overload
    def __init__(self) -> None: ...

    @typing.overload
    def __init__(self, pos: bool, cigar: bool, score: int, dis: int) -> None: ...

    @property
    def distance_filter(self) -> int:
        """
        :type: int
        """

    @distance_filter.setter
    def distance_filter(self, arg0: int) -> None:
        pass

    @property
    def report_begin_position(self) -> bool:
        """
        :type: bool
        """

    @report_begin_position.setter
    def report_begin_position(self, arg0: bool) -> None:
        pass

    @property
    def report_cigar(self) -> bool:
        """
        :type: bool
        """

    @report_cigar.setter
    def report_cigar(self, arg0: bool) -> None:
        pass

    @property
    def score_filter(self) -> int:
        """
        :type: int
        """

    @score_filter.setter
    def score_filter(self, arg0: int) -> None:
        pass

    pass

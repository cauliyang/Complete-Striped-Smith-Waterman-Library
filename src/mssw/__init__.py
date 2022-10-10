# !/usr/bin/env python
"""Init file for scannls package."""
__version__ = "0.0.1"
__PACKAGE_NAME__ = "mssw"

import mssw.cpp  # type: ignore
from mssw.cpp import Filter  # type: ignore

import typing as t
from rich.console import Console
from rich.table import Table

__all__ = ["Aligner", "Alignment", "Filter"]


class Alignment:
    """Alignment class."""

    def __init__(self):
        """Init."""
        self._alignment = cpp.Alignment()

    @classmethod
    def from_alignment(cls, alignment: "Alignment"):
        """Create alignment from cpp alignment."""
        new_alignment = cls()
        new_alignment._alignment = alignment
        return new_alignment

    @property
    def sw_score(self):
        """Get sw score."""
        return self._alignment.sw_score

    @property
    def sw_score_next_best(self):
        """Get sw score next best."""
        return self._alignment.sw_score_next_best

    @property
    def ref_begin(self):
        """Get ref begin."""
        return self._alignment.ref_begin

    @property
    def ref_end(self):
        """Get ref end."""
        return self._alignment.ref_end

    @property
    def query_begin(self):
        """Get query begin."""
        return self._alignment.query_begin

    @property
    def query_end(self):
        """Get query end."""
        return self._alignment.query_end

    @property
    def ref_end_next_best(self):
        """Get ref end next best."""
        return self._alignment.ref_end_next_best

    @property
    def mismatches(self):
        """Get mismatch."""
        return self._alignment.mismatches

    @property
    def cigar_string(self):
        """Get cigar string."""
        return self._alignment.cigar_string

    @property
    def cigar(self):
        """Get cigar."""
        return self._alignment.cigar

    def __repr__(self):
        """Get string representation."""
        return (
            f"Alignment(sw_score={self.sw_score}, sw_score_next_best={self.sw_score_next_best}, "
            f"ref_begin={self.ref_begin}, ref_end={self.ref_end}, query_begin={self.query_begin}, "
            f"query_end={self.query_end}, ref_end_next_best={self.ref_end_next_best}, "
            f"mismatch={self.mismatch}, cigar_string={self.cigar_string}, cigar={self.cigar})"
        )

    def clear(self):
        """Clear alignment."""
        self._alignment.clear()

    def print(self):
        """Print alignment."""
        table = Table(
            title="Alignment Result", show_header=True, header_style="bold magenta"
        )
        table.add_column("Metric", justify="left", style="cyan", no_wrap=True)
        table.add_column("Value", justify="right", style="magenta")

        table.add_row("sw_score", str(self.sw_score))
        table.add_row("sw_score_next_best", str(self.sw_score_next_best))
        table.add_row("ref_begin", str(self.ref_begin))
        table.add_row("ref_end", str(self.ref_end))
        table.add_row("query_begin", str(self.query_begin))
        table.add_row("query_end", str(self.query_end))
        table.add_row("ref_end_next_best", str(self.ref_end_next_best))
        table.add_row("mismatch", str(self.mismatches))
        table.add_row("cigar_string", str(self.cigar_string))
        table.add_row("cigar", str(self.cigar))

        console = Console()
        console.print(table)


class Aligner:
    def __init__(
        self, match: int = 2, mismatch: int = 2, gap_open: int = 3, gap_extend: int = 1
    ):
        self.match = match
        self.mismatch = mismatch
        self.gap_open = gap_open
        self.gap_extend = gap_extend
        self._aligner = cpp.Aligner(match, mismatch, gap_open, gap_extend)

    def align(
        self,
        query,
        ref,
        align_filter: t.Optional[cpp.Filter] = None,
        alignment: t.Optional[Alignment] = None,
    ) -> Alignment:
        if align_filter is None:
            align_filter = cpp.Filter()

        mask_len = max(len(ref) / 2, 15)

        if alignment is None:
            alignment = Alignment()

        self._aligner.Align(
            query, ref, len(ref), align_filter, alignment._alignment, int(mask_len)
        )
        return alignment

    def __repr__(self):
        return (
            f"Aligner(match={self.match}, mismatch={self.mismatch}, "
            f"gap_open={self.gap_open}, gap_extend={self.gap_extend})"
        )

    __str__ = __repr__

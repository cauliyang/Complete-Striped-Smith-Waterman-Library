# !/usr/bin/env python
"""Build cpp extension.

@Filename:    build.py
@author:      Yangyang Li
@Time:        1/7/22 3:00 PM
"""
import platform
import typing
from pathlib import Path

from pybind11.setup_helpers import Pybind11Extension, build_ext


def get_extra_options() -> typing.List[str]:
    pla_info = platform.platform().split("-")
    for item in pla_info:
        if item.startswith("arm"):
            return ["-march=armv8-a+fp+simd+crypto+crc"]
    return []


@typing.overload
def get_files(path: Path, suffix: typing.List[str]) -> typing.Iterator[str]:
    ...


@typing.overload
def get_files(path: str, suffix: typing.List[str]) -> typing.Iterator[str]:
    ...


def get_files(
    path: typing.Union[Path, str], suffix: typing.List[str]
) -> typing.Iterator[str]:
    """Get bindings."""
    if isinstance(path, str):
        path = Path(path)

    for file in path.iterdir():
        if file.is_dir():
            yield from get_files(file, suffix)
        if file.suffix in suffix:
            yield file.as_posix()


def build(setup_kwargs):
    """Build cpp extension."""
    ext_modules = [
        Pybind11Extension(
            "mssw",
            sources=["src/ssw.c", "src/ssw_cpp20.cpp"]
            + list(get_files("bindings", [".cpp", ".c"])),
            include_dirs=["src"],
            library_dirs=[],
            libraries=["m", "z"],
            extra_comiple_args=get_extra_options(),
        )
    ]
    setup_kwargs.update(
        {
            "ext_modules": ext_modules,
            "cmdclass": {"build_ext": build_ext},
            "zip_safe": False,
        }
    )

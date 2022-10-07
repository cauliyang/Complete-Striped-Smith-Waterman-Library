# YOU need to change

SOURCE := utils
BLOCK := arm sse emm
NAMESPACE := StripedSmithWaterman
INCLUDE := src/mssw/src

.PHONY: all_include clean bind help

help:
	@grep -E '^[a-zA-Z_-]+:.*?## .*$$' $(MAKEFILE_LIST) | sort | awk 'BEGIN {FS = ":.*?## "}; {printf "\033[36m%-30s\033[0m %s\n", $$1, $$2}'

all_include: ## make all include files into all_include.hpp
	python ${SOURCE}/generate_headers.py ${INCLUDE} ${BLOCK}

clean: ## clean previous binding directory
	rm -rf bindings && mkdir bindings

bind:  clean  all_include ## make bindings for python
	#https://cppbinder.readthedocs.io/en/latest/config.html
	docker run -it --rm -v `pwd`:/bind yangliz5/binder:1.0.1 \
	  binder --root-module _cpp \
	  --prefix /bind/bindings \
	  --bind ${NAMESPACE} \
	  /bind/all_includes.hpp \
	  -- -std=c++17 -I/bind/${INCLUDE} \
	  -I/usr/include \
	  -DMY_PROJECT_DEFINE -DNDEBUG

	 echo "Binding Cpp to python"

#!/bin/sh

required_packages="python3"
required_build_packages="git gcc musl-dev make python3-dev py3-pip"
installed_packages=$(apk info)

contains()
{
	# Check if >>list<< contains >>elem<<
    elem=$1
    list=$2
    for lelem in $list; do
        if [[ $lelem == $elem ]]; then
            return 0
        fi
    done
    return 1
}

cleanup()
{
	# Cleanup build dependency packages that were not already
	# installed on the system
	for pkg in $required_build_packages; do
		if ! $(contains $pkg "$installed_packages"); then
			apk del $pkg
		fi
	done
}

# Install required packages
apk add $required_packages
apk add $required_build_packages

# Fetch libdwarf
cd /tmp/
wget https://github.com/davea42/libdwarf-code/releases/download/v0.3.4/libdwarf-0.3.4.tar.xz
tar xf libdwarf-0.3.4.tar.xz

# Build libdwarf
mkdir /tmp/build
cd /tmp/build
/tmp/libdwarf-0.3.4/configure --enable-shared --disable-static
make
make install

# Create symlinks for libdwarf headers
mkdir /usr/local/include/libdwarf
ln -s /usr/local/include/libdwarf-0/dwarf.h /usr/local/include/libdwarf/dwarf.h
ln -s /usr/local/include/libdwarf-0/libdwarf.h /usr/local/include/libdwarf/libdwarf.h

# Clone funsack
cd /tmp/
git clone https://github.com/MNayer/funsack.git
cd /tmp/funsack
python3 -m pip install .

# Remove build artifacts
rm -rf /tmp/funsack
rm -rf /tmp/libdwarf*
rm -rf /tmp/build

# Remove build packages
apk del git gcc musl-dev make python3-dev py3-pip

# Cleanup build packages
cleanup

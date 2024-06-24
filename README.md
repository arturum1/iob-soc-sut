# IOb-SoC-SUT

IOb-SoC-SUT is a generic RISC-V SoC, based on [IOb-SoC-OpenCryptoLinux](https://github.com/IObundle/iob-soc-opencryptolinux), used for verification by the [OpenCryptoTester](https://nlnet.nl/project/OpenCryptoTester#ack) project.
This repository contains an example System Under Test (SUT) and a Tester to verify it. The SUT is used to demonstrate the Tester's abilities for verification purposes.

The SUT runs on bare metal and has UART, GPIO, AXI4-Stream, and IOb-native interfaces.

## Table of Contents
- [OpenCryptoTester](https://github.com/IObundle/iob-soc-sut#opencryptotester)
    - [Dependencies](https://github.com/IObundle/iob-soc-sut#dependencies)
    - [Nix Environment](https://github.com/IObundle/iob-soc-sut#nix-environment)
    - [Setup SUT](https://github.com/IObundle/iob-soc-sut#setup-the-sut)
        - [Emulate SUT](https://github.com/IObundle/iob-soc-sut#emulate-the-sut-on-the-pc)
        - [Simulate SUT](https://github.com/IObundle/iob-soc-sut#simulate-the-sut)
        - [Build and run on FPGA](https://github.com/IObundle/iob-soc-sut#build-and-run-the-sut-on-the-fpga-board)
    - [Setup Tester](https://github.com/IObundle/iob-soc-sut#setup-the-tester-along-with-the-sut)
        - [Build and run Tester](https://github.com/IObundle/iob-soc-sut#build-and-run-the-tester-along-with-the-sut)
    - [Cleaning](https://github.com/IObundle/iob-soc-sut#cleaning)
    - [Setup SUT as netlist](https://github.com/IObundle/iob-soc-sut#setup-the-sut-as-a-netlist)
- [OpenCryptoTester with Generic UUT](https://github.com/IObundle/iob-soc-sut#instructions-to-configure-the-opencryptotester-with-a-generic-uut)
    - [UUT's Repository Minimum Requirements](https://github.com/IObundle/iob-soc-sut#uuts-repository-minimum-requirements)
    - [Clone the IOb-SoC-OpenCryptoLinux's repository](https://github.com/IObundle/iob-soc-sut#clone-the-iob-soc-opencryptolinuxs-repository)
    - [Configure the Tester](https://github.com/IObundle/iob-soc-sut#configure-the-tester)
    - [Setup, build and run the Tester](https://github.com/IObundle/iob-soc-sut#setup-build-and-run-the-tester-along-with-uut)
    - [Add a new Device to be tested](https://github.com/IObundle/iob-soc-sut#add-a-new-device-to-be-tested)
    - [Cleaning](https://github.com/IObundle/iob-soc-sut#cleaning)
- [Installing the RISC-V GNU Compiler Toolchain](https://github.com/IObundle/iob-soc-sut#instructions-for-installing-the-risc-v-gnu-compiler-toolchain)
- [Acknowledgement](https://github.com/IObundle/iob-soc-sut#acknowledgement)

# [OpenCryptoTester](https://nlnet.nl/project/OpenCryptoTester#ack)

The [OpenCryptoTester](https://nlnet.nl/project/OpenCryptoTester#ack) project aims to develop a System-on-Chip (SoC) used mainly to verify cryptographic systems that improve internet security but can also be used on any SoC. It is synergetic with several other NGI Assure-funded open-source projects - notably [OpenCryptoHW](https://nlnet.nl/project/OpenCryptoHW) (Coarse-Grained Reconfigurable Array cryptographic hardware) and [OpenCryptoLinux](https://nlnet.nl/project/OpenCryptoLinux). The proposed SoC will support test instruments as peripherals and use OpenCryptoHW as the System Under Test (SUT), hopefully opening the way for open-source test instrumentation operated under Linux.

The Tester SoC is also based on [IOb-SoC-OpenCryptoLinux](https://github.com/IObundle/iob-soc-opencryptolinux) and its configuration is stored in the `submodules/TESTER` directory.

The Tester system is compatible with any Unit Under Tester (UUT) as it does not impose any hardware constraints.
Nonetheless, the UUT's repository must follow the [set of minimum requirements](#uuts-repository-minimum-requirements) presented below.

For details on configuring, building, and running the Tester with a generic UUT check out [this section](#instructions-to-configure-the-opencryptotester-with-a-generic-uut).

## Dependencies

Before building the system, install the following tools:
- GNU Bash >=5.1.16
- GNU Make >=4.3
- RISC-V GNU Compiler Toolchain =2022.06.10  (Instructions at the end of this README)
- Python3 >=3.10.6
- Python3-Parse >=1.19.0

Optional tools, depending on the desired run strategy:
- Icarus Verilog >=10.3
- Verilator >=5.002
- gtkwave >=3.3.113
- Vivado >=2020.2
- Quartus >=20.1

Older versions of the dependencies above may work but still need to be tested.

## Nix environment

Instead of manually installing the dependencies above, you can use
[nix-shell](https://nixos.org/download.html#nix-install-linux) to run
IOb-SoC-SUT+Tester in a [Nix](https://nixos.org/) environment with all dependencies
available except for Vivado and Quartus.

- Run `nix-shell` from the IOb-SoC-SUT root directory to install and start the environment with all the required dependencies.

## Setup the SUT

This system's setup, build, and run steps are similar to the ones used in [IOb-SoC-OpenCryptoLinux](https://github.com/IObundle/iob-soc-opencryptolinux).
Check the `README.md` file of that repository for more details on the process of setup, building, and running the system without the Tester.

The SUT's main configuration, stored in `iob_soc_sut.py`, sets the UART, GPIO, REGFILEIF, AXISTREAM, and ETHERNET peripherals. In total, the SUT has one UART, one GPIO, one ETHERNET, one AXISTREAMIN, one AXISTREAMOUT, and one IOb-native (provided by the REGFILEIF peripheral) interface.

To set up the system, type:

```Bash
make setup [<control parameters>]
```

`<control parameters>` are system configuration parameters passed in the
command line, overriding those inherited from the system. Example control
parameters are `INIT_MEM=0`. For example:

```Bash
make setup INIT_MEM=0
```

The setup process will create a build directory that contains all the files required for building the system.

The **setup directory** is considered to be the repository folder, as it contains the files needed to set up the system.

The **build directory** is considered to be the folder generated by the setup process, as it contains the files needed to build the system.
The build directory is usually located in `../iob_soc_sut_V*` relative to the setup directory.

The SUT's firmware, stored in `software/firmware/iob_soc_sut_firmware.c` has two modes of operation:
- Without external memory (USE\_EXTMEM=0) (Temporarily disabled due to lack of compatibility with iob-eth)
- Running from external memory (USE\_EXTMEM=1)

Note: The iob-eth core has a native DMA interface, so the SUT only uses this core when running with external memory.

When running without external memory, the SUT only prints a few `Hello Word!` messages via UART, reads GPIO inputs and sets its outputs. It also reads and inserts values into the registers of its IOb-native interface. Also, it receives a stream of bytes via its input AXI4-Stream interface, prints them, and sends them back via its output AXI4-Stream interface.

When running from the external memory, the system does the same as without external memory. 
Besides that, it transfers the `eth_example.txt` file via ethernet from the PC, stores its contents in memory via DMA, and prints them.
It also allocates and stores a string in memory and writes its pointer to a register in the IOb-native interface.

### Emulate the SUT on the PC

To emulate the SUT's embedded software on a PC, type:

```Bash
make -C ../iob_soc_sut_V* pc-emul
```

### Simulate the SUT

To build and run the SUT in simulation, type:

```Bash
make -C ../iob_soc_sut_V* sim-run [SIMULATOR=<simulator name>]
```

`<simulator name>` is the name of the simulator's Makefile segment.

### Build and run the SUT on the FPGA board

To build the SUT for FPGA, type:

```Bash
make -C ../iob_soc_sut_V* fpga-build [BOARD=<board directory name>]
```

`<board directory name>` is the name of the board's run directory.

To run the SUT in FPGA, type:

```Bash
make -C ../iob_soc_sut_V* fpga-run [BOARD=<board directory name>]
```

## Setup the Tester along with the SUT

The Tester's configuration is stored in the `submodules/TESTER/iob_soc_tester.py` Python module.
It adds the SUT (via IOb-native interface), one AXISTREAMIN, one AXISTREAMOUT, two ETHERNET, one GPIO, one ILA (with internal Monitor), one PFSM, one DMA, and a second UART instance to the list of Tester peripherals.
In total, the Tester has two UART interfaces, two ETHERNET, one GPIO, one AXISTREAMIN, one AXISTREAMOUT, one ILA, one PFSM, one DMA, and the SUT as peripherals.

To set up the Tester with the SUT, type:

```Bash
make setup TESTER=1 [<control parameters>]
```

The SUT and Tester's peripheral IO connections, stored in the `peripheral_portmap` list of the `iob_soc_tester` class, have the following configuration:
- Instance 0 of Tester's UART is connected to the PC's console.
- Instance 1 of Tester's UART is connected to the SUT's UART.
- Instance 0 of Tester's ETHERNET is connected to the PC's console.
- Instance 1 of Tester's ETHERNET is connected to the SUT's ETHERNET.
- Tester's GPIO is connected to SUT's GPIO.
- Tester's AXISTREAMOUT is connected to SUT's AXISTREAMIN.
- Tester's AXISTREAMIN is connected to SUT's AXISTREAMOUT.
- Tester's ILA is connected to internal Tester wires, that connect to probes auto-generated by the ILA peripheral.
- Tester's PFSM is connected to internal Tester wires, that connect to probes auto-generated by the ILA peripheral.
- Tester's AXISTREAMIN DMA interface is connected to Tester's DMA peripheral.
- Tester's AXISTREAMOUT DMA interface is connected to Tester's DMA peripheral.
- Tester's ILA DMA interface is connected to Tester's DMA peripheral.

Note: The IOb-native interface of the SUT (provided by the REGFILEIF peripheral) is automatically connected to the Tester's peripheral bus.

The Tester's firmware, stored in `software/firmware/iob_soc_tester_firmware.c`, also has two modes of operation:
- Without external memory (USE\_EXTMEM=0) (Temporarily disabled due to lack of compatibility with iob-eth)
- Running from external memory (USE\_EXTMEM=1)

Note: The iob-eth core has a native DMA interface, so the Tester only uses this core when running with external memory.

When running without external memory, the Tester relays messages printed from the SUT via UART to the console and set/read values from the IOb-native interface connected to the SUT.
It also sets and reads values from the GPIO interface, transfers byte streams using its AXI4-Stream interfaces, and it reads internal signals from the SUT using the probes of the ILA peripheral.
It also uses the ILA peripheral to sample values from the PFSM output and uses the ILA's internal Monitor core to enable the sampling during 4 clocks starting at each trigger.
The Tester uses its DMA peripheral to transfer most of the data of its AXISTREAM peripherals, directly to/from the external memory.

When running from the external memory, the Tester does the same as without external memory.
Besides that, it transfers the `eth_example.txt` file via ethernet from the PC, stores its contents in memory via DMA, and prints them.
It also sends a few test bytes via ethernet to the SUT's ethernet interface.
It also reads a string pointer from the IOb-native interface.
It inverts the most significant bit of that pointer to access the SUT's address space and then reads the string stored at that location.

More details on configuring, building, and running the Tester are available in the [section with instructions for Tester with a generic UUT](#instructions-to-configure-the-opencryptotester-with-a-generic-uut).

### Build and run the Tester along with the SUT

The steps to build and run the Tester along with the SUT, are the same as the ones for the SUT individually.
You just need to make sure that the system was previously set up with the `TESTER=1` argument in the `make setup TESTER=1` command.

To build and run in simulation, type:

```Bash
make -C ../iob_soc_sut_V* sim-run [SIMULATOR=<simulator name>]
```

`<simulator name>` is the name of the simulator's Makefile segment.

To build for FPGA, type:

```Bash
make -C ../iob_soc_sut_V* fpga-build [BOARD=<board name>]
```

`<board name>` is the name of the board's run directory.

To run in FPGA, type:

```Bash
make -C ../iob_soc_sut_V* fpga-run [BOARD=<board name>]
```

## Cleaning

The following command will clean the selected simulation, board, and document
directories, locally and in the remote servers:

```Bash
make -C ../iob_soc_sut_V* clean
```

The following command will delete the build directory:

```Bash
make clean
```

## Setup the SUT as a netlist

The SUT can be individually built as a netlist by running the following Makefile target:

```Bash
make build-sut-netlist [BOARD=<board name>]
```

`<board name>` is the name of the board's run directory.
The FPGA board will not be used during this stage.
This variable is used to select which tool and server to use.

This netlist can then be combined with the Tester for verification.

To build the SUT's netlist, attach it to the Tester and run it on the FPGA, type:

```Bash
make tester-sut-netlist [BOARD=<board name>] [<control parameters>]
```

`<control parameters>` are system configuration parameters passed in the command line, overriding those in the `iob_soc_sut_setup.py` file.

Check the commands run by the `tester-sut-netlist` Makefile target for more details.

This command will generate the `../iob_soc_tester_V*/` build folder.
To delete it, type:

```Bash
rm -r ../iob_soc_tester_V*
```

# Instructions to configure the OpenCryptoTester with a generic UUT
instructions-to-configure-the-opencryptotester-with-a-generic-uut

## UUT's Repository Minimum Requirements

The Unit Under Test (UUT) repository must contain at least the `<uut_name>.py` file to be compatible with the OpenCryptoTester.

Note: The IOb-SoC-SUT system in this repository is an example UUT, and already meets these requirements.

The `<uut_name>.py` python module provides the Tester with information about the UUT, and should contain the following objects:

- Must contain a class describing the UUT's Verilog module, with the following attributes:
    - The `name` string.
    - The `confs` list.
    - The `ios` list.
    - The `instance(...)` method.

### name

The `name` attribute should contain a string equal to the name of the UUT's Verilog top module.

### confs

The `confs` attribute should be a list with a similar structure to the one in the `iob_soc_opencryptolinux.py` file of the [IOb-SoC-OpenCryptoLinux](https://github.com/IObundle/iob-soc-opencryptolinux) system.
This list informs the Tester of the parameters available for the UUT's Verilog top module.

### ios

The `ios` attribute should be a list with a similar structure to the one in the `iob_soc_opencryptolinux.py` file of the [IOb-SoC-OpenCryptoLinux](https://github.com/IObundle/iob-soc-opencryptolinux) system.
This list informs the Tester of the IOs in the UUT's Verilog top module.

### instance(...)

The `instance(...)` method should be similar to the one defined in the `iob_module` class.
This method returns an `iob_verilog_instance` class that informs the Tester of details related to the Verilog instance.

## Clone the IOb-SoC-OpenCryptoLinux's repository

Since the Tester is based on the IOb-SoC-OpenCryptoLinux system, it needs the contents from its repository to be set up correctly.

If the UUT's repository is git based, then we suggest adding the IOb-SoC-OpenCryptoLinux's repository as a git submodule.

To add the IOb-SoC-OpenCryptoLinux's repository as a git submodule inside the `submodules/` folder, from the UUT's repository, run:

```Bash
git submodule add git@github.com:IObundle/iob-soc-opencryptolinux.git submodules/IOBSOC
git submodule update --init --recursive
```

Otherwise, clone the IOb-SoC-OpenCryptoLinux's repository to a location of your choosing with the following command:

```Bash
git clone --recursive git@github.com:IObundle/iob-soc-opencryptolinux.git
```

## Configure the Tester

The Tester's setup, build and run steps are similar to the ones used in [IOb-SoC-OpenCryptoLinux](https://github.com/IObundle/iob-soc-opencryptolinux).
Check the `README.md` file of that repository for more details on the process of setup, building, and running IOb-SoC-based systems.

The Tester's configuration is stored in the `iob_soc_tester.py` Python module.
The user should create this file according to the UUT's project requirements.
This file must define the `iob_soc_tester` class for the Tester that is a subclass of the `iob_soc_opencryptolinux` class, available in the [`iob_soc_opencryptolinux.py`](https://github.com/IObundle/iob-soc-opencryptolinux/blob/master/iob_soc_opencryptolinux.py).
Therefore the attributes and methods of the IOb-SoC-OpenCryptoLinux system are inherited by the Tester.

The Tester-specific configuration in the `iob_soc_tester` class can modify any attributes or methods inherited from the IOb-SoC-OpenCryptoLinux system, according to its needs for verification of the UUT.

When creating a new Tester for a generic UUT, the modifications typically required for the `iob_soc_tester.py` python module are:

1. Add verification peripherals:
    1. Import the peripheral's Python module containing their class.
    2. Run the `setup()` method of that peripheral to set up the peripheral's Verilog module (Method inherited from the iob_module class).
    3. Run the `instance()` method of that peripheral to set up the peripheral's Verilog instances (Method inherited from the iob_module class).
    4. Add an entry to the `peripheral_portmap` list for each IO of the peripheral instance. Each entry defines where to connect the peripheral IO ports. Each entry may map a single bit, selected bits, an entire port, or an entire interface.
2. Add the UUT as a peripheral of the Tester using the steps from 1.
3. Optionally, modify the inherited IOb-SoC-OpenCryptoLinux attributes or methods according to the project. For example, update the default memory size using the `_setup_confs()` method inherited from IOb-SoC-OpenCryptoLinux.

For reference, check out the `submodules/TESTER/iob_soc_tester.py` Python module.

## Setup, build, and run the Tester along with UUT

With the UUT's minimum requirements fulfilled, the steps for setup, building, and running the Tester are similar to those of [IOb-SoC-OpenCryptoLinux](https://github.com/IObundle/iob-soc-opencryptolinux).

First, create a Makefile that includes the `setup.mk` makefile segment from [IOb-Lib](https://github.com/IObundle/iob-lib).

After that, run the following Makefile commands to build the Tester:

To set up the Tester, type:

```Bash
make setup TOP_MODULE_NAME=iob_soc_tester [<control parameters>]

```
`TOP_MODULE_NAME` is a Makefile variable used by the `setup.mk` makefile segment to select the top module.

`<control parameters>` are system configuration parameters passed in the
command line, overriding those in the `iob_soc_tester_setup.py` file. Example control
parameters are `INIT_MEM=0`.

To build and run the Tester in simulation, type:

```Bash
make -C ../iob_soc_tester_V* sim-run [SIMULATOR=<simulator name>]
```

`<simulator name>` is the name of the simulator's Makefile segment.

To build the Tester for the FPGA, type:

```Bash
make -C ../iob_soc_tester_V* fpga-build [BOARD=<board directory name>]
```

`<board directory name>` is the name of the board's run directory.

To run the Tester in the FPGA, type:

```Bash
make -C ../iob_soc_tester_V* fpga-run [BOARD=<board directory name>]
```

## Add a new Device to be tested
Checkout [this tutorial](document/new_device_tutorial.md) for more details on
how to add a new device to be tested.

## Cleaning

The following command will clean the selected simulation, board, and document
directories, locally and in the remote servers:

```Bash
make -C ../iob_soc_tester_V* clean
```

The following command will delete the build directory:

```Bash
make clean
```

# Instructions for Installing the RISC-V GNU Compiler Toolchain

The scripts to build the RISC-V GNU Compiler Toolchain are already included in the Nix environment, described in the [nix environment section](#nix-environment).
As an alternative, you can manually install the RISC-V GNU Compiler Toolchain, using the commands below.

### Get sources and check out the supported stable version

```Bash
git clone https://github.com/riscv/riscv-gnu-toolchain
cd riscv-gnu-toolchain
git checkout 2022.06.10
```

### Prerequisites

For the Ubuntu OS and its variants:

```Bash
sudo apt install autoconf automake autotools-dev curl python3 python2 libmpc-dev libmpfr-dev libgmp-dev gawk build-essential bison flex texinfo gperf libtool patchutils bc zlib1g-dev libexpat-dev
```

For CentOS and its variants:

```Bash
sudo yum install autoconf automake python3 python2 libmpc-devel mpfr-devel gmp-devel gawk  bison flex texinfo patchutils gcc gcc-c++ zlib-devel expat-devel
```

### Installation

```Bash
./configure --prefix=/path/to/riscv --enable-multilib
sudo make -j$(nproc)
```

This will take a while. After it is done, type:

```Bash
export PATH=$PATH:/path/to/riscv/bin
```

The above command should be added to your `~/.bashrc` file, so that
you do not have to type it on every session.

# Acknowledgement
The [OpenCryptoTester](https://nlnet.nl/project/OpenCryptoTester#ack) project is funded through the NGI Assure Fund, a fund established by NLnet
with financial support from the European Commission's Next Generation Internet
programme, under the aegis of DG Communications Networks, Content and Technology
under grant agreement No 957073.

<table>
    <tr>
        <td align="center" width="50%"><img src="https://nlnet.nl/logo/banner.svg" alt="NLnet foundation logo" style="width:90%"></td>
        <td align="center"><img src="https://nlnet.nl/image/logos/NGIAssure_tag.svg" alt="NGI Assure logo" style="width:90%"></td>
    </tr>
</table>

/*
Copyright (c) 2009-2012, Intel Corporation
All rights reserved.
Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
    * Neither the name of Intel Corporation nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
// written by Roman Dementiev,
//            Pat Fay
//        Austen Ott
//            Jim Harris (FreeBSD)

#include <iostream>
#include <sstream>
#include <iomanip>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "pci.h"

#ifndef _MSC_VER
    #include <sys/mman.h>
    #include <errno.h>
#endif

#ifdef _MSC_VER

#include <windows.h>
#include "Winmsrdriver\win7\msrstruct.h"
#include "winring0/OlsDef.h"
#include "winring0/OlsApiInitExt.h"

extern HMODULE hOpenLibSys;

PciHandle::PciHandle(uint32 groupnr_, uint32 bus_, uint32 device_, uint32 function_) :
    bus(bus_),
    device(device_),
    function(function_),
    pciAddress(PciBusDevFunc(bus_, device_, function_))
{
    if (groupnr_ != 0) {
        std::cerr << "Non-zero PCI group segments are not supported in PCM/Windows" << std::endl;
        throw std::exception();
    }

    hDriver = CreateFile("\\\\.\\RDMSR", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);

    if (hDriver == INVALID_HANDLE_VALUE && hOpenLibSys == NULL)
        throw std::exception();
}

bool PciHandle::exists(uint32 bus_, uint32 device_, uint32 function_)
{
    if (hOpenLibSys != NULL) return true;

    HANDLE tempHandle = CreateFile("\\\\.\\RDMSR", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);

    if (tempHandle == INVALID_HANDLE_VALUE)
        return false;

    // TODO: check device availability
    CloseHandle(tempHandle);
    return true;
}

int32 PciHandle::read32(uint64 offset, uint32 * value)
{
    if (hDriver != INVALID_HANDLE_VALUE) {
        PCICFG_Request req;
        ULONG64 result = 0;
        DWORD reslength = 0;
        req.bus = bus;
        req.dev = device;
        req.func = function;
        req.bytes = sizeof(uint32);
        req.reg = (uint32)offset;
        BOOL status = DeviceIoControl(hDriver, IO_CTL_PCICFG_READ, &req, sizeof(PCICFG_Request), &result, sizeof(uint64), &reslength, NULL);
        *value = (uint32)result;

        if (!status) {
            //std::cerr <<"Error reading PCI Config space at bus"<<bus<<"dev"<< device<<"function"<< function <<"offset"<< offset << "size"<< req.bytes  << ". Windows error:"<<GetLastError()<<std::endl;
        }

        return reslength;
    }

    DWORD result = 0;

    if (ReadPciConfigDwordEx(pciAddress, (DWORD)offset, &result)) {
        *value = result;
        return sizeof(uint32);
    }

    return 0;
}

int32 PciHandle::write32(uint64 offset, uint32 value)
{
    if (hDriver != INVALID_HANDLE_VALUE) {
        PCICFG_Request req;
        ULONG64 result;
        DWORD reslength = 0;
        req.bus = bus;
        req.dev = device;
        req.func = function;
        req.bytes = sizeof(uint32);
        req.reg = (uint32)offset;
        req.write_value = value;
        BOOL status = DeviceIoControl(hDriver, IO_CTL_PCICFG_WRITE, &req, sizeof(PCICFG_Request), &result, sizeof(uint64), &reslength, NULL);

        if (!status) {
            //std::cerr <<"Error writing PCI Config space at bus"<<bus<<"dev"<< device<<"function"<< function <<"offset"<< offset << "size"<< req.bytes  << ". Windows error:"<<GetLastError()<<std::endl;
        }

        return reslength;
    }

    return (WritePciConfigDwordEx(pciAddress, (DWORD)offset, value)) ? sizeof(uint32) : 0;
}

int32 PciHandle::read64(uint64 offset, uint64 * value)
{
    if (hDriver != INVALID_HANDLE_VALUE) {
        PCICFG_Request req;
        // ULONG64 result;
        DWORD reslength = 0;
        req.bus = bus;
        req.dev = device;
        req.func = function;
        req.bytes = sizeof(uint64);
        req.reg = (uint32)offset;
        BOOL status = DeviceIoControl(hDriver, IO_CTL_PCICFG_READ, &req, sizeof(PCICFG_Request), value, sizeof(uint64), &reslength, NULL);

        if (!status) {
            //std::cerr <<"Error reading PCI Config space at bus"<<bus<<"dev"<< device<<"function"<< function <<"offset"<< offset << "size"<< req.bytes  << ". Windows error:"<<GetLastError()<<std::endl;
        }

        return reslength;
    }

    cvt_ds cvt;
    cvt.ui64 = 0;
    BOOL status = ReadPciConfigDwordEx(pciAddress, (DWORD)offset, &(cvt.ui32.low));
    status &= ReadPciConfigDwordEx(pciAddress, ((DWORD)offset) + sizeof(uint32), &(cvt.ui32.high));

    if (status) {
        *value = cvt.ui64;
        return sizeof(uint64);
    }

    return 0;
}

int32 PciHandle::write64(uint64 offset, uint64 value)
{
    if (hDriver != INVALID_HANDLE_VALUE) {
        PCICFG_Request req;
        ULONG64 result;
        DWORD reslength = 0;
        req.bus = bus;
        req.dev = device;
        req.func = function;
        req.bytes = sizeof(uint64);
        req.reg = (uint32)offset;
        req.write_value = value;
        BOOL status = DeviceIoControl(hDriver, IO_CTL_PCICFG_WRITE, &req, sizeof(PCICFG_Request), &result, sizeof(uint64), &reslength, NULL);

        if (!status) {
            //std::cerr <<"Error writing PCI Config space at bus"<<bus<<"dev"<< device<<"function"<< function <<"offset"<< offset << "size"<< req.bytes  << ". Windows error:"<<GetLastError()<<std::endl;
        }

        return reslength;
    }

    cvt_ds cvt;
    cvt.ui64 = value;
    BOOL status = WritePciConfigDwordEx(pciAddress, (DWORD)offset, cvt.ui32.low);
    status &= WritePciConfigDwordEx(pciAddress, ((DWORD)offset) + sizeof(uint32), cvt.ui32.high);
    return status ? sizeof(uint64) : 0;
}

PciHandle::~PciHandle()
{
    if (hDriver != INVALID_HANDLE_VALUE) CloseHandle(hDriver);
}

#elif __APPLE__
//OSX does not run on any JKT processors, so the PciHandle class will never be used.

PciHandle::PciHandle(uint32 groupnr_, uint32 bus_, uint32 device_, uint32 function_) :
    bus(bus_),
    device(device_),
    function(function_)
{
    fprintf(stderr, "Attempting to use PciHandle on OS X, this is not supported.\n");
    assert(0 == 1);
}

bool PciHandle::exists(uint32 bus_, uint32 device_, uint32 function_)
{
    fprintf(stderr, "Attempting to use PciHandle on OS X, this is not supported.\n");
    assert(0 == 1);
    return false;
}

int32 PciHandle::read32(uint64 offset, uint32 * value)
{
    fprintf(stderr, "Attempting to use PciHandle on OS X, this is not supported.\n");
    assert(0 == 1);
    return -1;
}

int32 PciHandle::write32(uint64 offset, uint32 value)
{
    fprintf(stderr, "Attempting to use PciHandle on OS X, this is not supported.\n");
    assert(0 == 1);
    return -1;
}

int32 PciHandle::read64(uint64 offset, uint64 * value)
{
    fprintf(stderr, "Attempting to use PciHandle on OS X, this is not supported.\n");
    assert(0 == 1);
    return -1;
}

int32 PciHandle::write64(uint64 offset, uint64 value)
{
    fprintf(stderr, "Attempting to use PciHandle on OS X, this is not supported.\n");
    assert(0 == 1);
    return -1;
}

PciHandle::~PciHandle()
{
    fprintf(stderr, "Attempting to use PciHandle on OS X, this is not supported.\n");
    assert(0 == 1);
}

#elif (defined __FreeBSD__)

#include <sys/pciio.h>

PciHandle::PciHandle(uint32 groupnr_, uint32 bus_, uint32 device_, uint32 function_) :
    fd(-1),
    bus(bus_),
    device(device_),
    function(function_)
{
    if (groupnr_ != 0) {
        std::cout << "ERROR: non-zero PCI segment groupnr is not supported in this PciHandle implementation" << std::endl;
        throw std::exception();
    }

    int handle = ::open("/dev/pci", O_RDWR);

    if (handle < 0) throw std::exception();

    fd = handle;
}

bool PciHandle::exists(uint32 bus_, uint32 device_, uint32 function_)
{
    struct pci_conf_io pc;
    struct pci_match_conf pattern;
    struct pci_conf conf[4];
    int fd;
    fd = ::open("/dev/pci", O_RDWR, 0);

    if (fd < 0) return false;

    bzero(&pc, sizeof(pc));
    pattern.pc_sel.pc_bus = bus_;
    pattern.pc_sel.pc_dev = device_;
    pattern.pc_sel.pc_func = function_;
    pattern.flags = (pci_getconf_flags)(PCI_GETCONF_MATCH_BUS | PCI_GETCONF_MATCH_DEV | PCI_GETCONF_MATCH_FUNC);
    pc.pat_buf_len = sizeof(pattern);
    pc.patterns = &pattern;
    pc.num_patterns = 1;
    pc.match_buf_len = sizeof(conf);
    pc.matches = conf;

    if (ioctl(fd, PCIOCGETCONF, &pc)) return false;

    if (pc.status != PCI_GETCONF_LAST_DEVICE) return false;

    if (pc.num_matches > 0) return true;

    return false;
}

int32 PciHandle::read32(uint64 offset, uint32 * value)
{
    struct pci_io pi;
    int ret;
    pi.pi_sel.pc_domain = 0;
    pi.pi_sel.pc_bus = bus;
    pi.pi_sel.pc_dev = device;
    pi.pi_sel.pc_func = function;
    pi.pi_reg = offset;
    pi.pi_width = 4;
    ret = ioctl(fd, PCIOCREAD, &pi);

    if (!ret) *value = pi.pi_data;

    return ret;
}

int32 PciHandle::write32(uint64 offset, uint32 value)
{
    struct pci_io pi;
    pi.pi_sel.pc_domain = 0;
    pi.pi_sel.pc_bus = bus;
    pi.pi_sel.pc_dev = device;
    pi.pi_sel.pc_func = function;
    pi.pi_reg = offset;
    pi.pi_width = 4;
    pi.pi_data = value;
    return ioctl(fd, PCIOCWRITE, &pi);
}

int32 PciHandle::read64(uint64 offset, uint64 * value)
{
    struct pci_io pi;
    int32 ret;
    pi.pi_sel.pc_domain = 0;
    pi.pi_sel.pc_bus = bus;
    pi.pi_sel.pc_dev = device;
    pi.pi_sel.pc_func = function;
    pi.pi_reg = offset;
    pi.pi_width = 4;
    ret = ioctl(fd, PCIOCREAD, &pi);

    if (ret) return ret;

    *value = pi.pi_data;
    pi.pi_reg += 4;
    ret = ioctl(fd, PCIOCREAD, &pi);

    if (ret) return ret;

    *value += ((uint64)pi.pi_data << 32);
    return 0;
}

int32 PciHandle::write64(uint64 offset, uint64 value)
{
    struct pci_io pi;
    int32 ret;
    pi.pi_sel.pc_domain = 0;
    pi.pi_sel.pc_bus = bus;
    pi.pi_sel.pc_dev = device;
    pi.pi_sel.pc_func = function;
    pi.pi_reg = offset;
    pi.pi_width = 8;
    pi.pi_data = (value & (1LL << 32) - 1);
    ret = ioctl(fd, PCIOCWRITE, &pi);

    if (ret) return ret;

    pi.pi_reg += 4;
    pi.pi_data = (value >> 32);
    return ioctl(fd, PCIOCWRITE, &pi);
}

PciHandle::~PciHandle()
{
    if (fd >= 0) ::close(fd);
}

#else


// Linux implementation

PciHandle::PciHandle(uint32 groupnr_, uint32 bus_, uint32 device_, uint32 function_) :
    fd(-1),
    bus(bus_),
    device(device_),
    function(function_)
{
    if (groupnr_ != 0) {
        std::cout << "ERROR: non-zero PCI segment groupnr is not supported in this PciHandle implementation" << std::endl;
        throw std::exception();
    }

    std::ostringstream path(std::ostringstream::out);
    path << std::hex << "/proc/bus/pci/" << std::setw(2) << std::setfill('0') << bus << "/" << std::setw(2) << std::setfill('0') << device << "." << function;
    // std::cout <<path.str().c_str() << std::endl;
    int handle = ::open(path.str().c_str(), O_RDWR);

    if (handle < 0) throw std::exception();

    fd = handle;
    // std::cout <<"Opened"<< path.str().c_str() << "on handle"<< fd << std::endl;
}


bool PciHandle::exists(uint32 bus_, uint32 device_, uint32 function_)
{
    std::ostringstream path(std::ostringstream::out);
    path << std::hex << "/proc/bus/pci/" << std::setw(2) << std::setfill('0') << bus_ << "/" << std::setw(2) << std::setfill('0') << device_ << "." << function_;
    int handle = ::open(path.str().c_str(), O_RDWR);

    if (handle < 0) return false;

    ::close(handle);
    return true;
}

int32 PciHandle::read32(uint64 offset, uint32 * value)
{
    return ::pread(fd, (void *)value, sizeof(uint32), offset);
}

int32 PciHandle::write32(uint64 offset, uint32 value)
{
    return ::pwrite(fd, (const void *)&value, sizeof(uint32), offset);
}

int32 PciHandle::read64(uint64 offset, uint64 * value)
{
    return ::pread(fd, (void *)value, sizeof(uint64), offset);
}

int32 PciHandle::write64(uint64 offset, uint64 value)
{
    return ::pwrite(fd, (const void *)&value, sizeof(uint64), offset);
}

PciHandle::~PciHandle()
{
    if (fd >= 0) ::close(fd);
}

#ifndef PCM_USE_PCI_MM_LINUX

PciHandleM::PciHandleM(uint32 bus_, uint32 device_, uint32 function_) :
    fd(-1),
    bus(bus_),
    device(device_),
    function(function_),
    base_addr(0)
{
    int handle = ::open("/dev/mem", O_RDWR);

    if (handle < 0) throw std::exception();

    fd = handle;
    int mcfg_handle = ::open("/sys/firmware/acpi/tables/MCFG", O_RDONLY);

    if (mcfg_handle < 0) throw std::exception();

    int32 result = ::pread(mcfg_handle, (void *)&base_addr, sizeof(uint64), 44);

    if (result != sizeof(uint64)) {
        ::close(mcfg_handle);
        throw std::exception();
    }

    unsigned char max_bus = 0;
    result = ::pread(mcfg_handle, (void *)&max_bus, sizeof(unsigned char), 55);

    if (result != sizeof(unsigned char)) {
        ::close(mcfg_handle);
        throw std::exception();
    }

    ::close(mcfg_handle);

    if (bus > (unsigned)max_bus) {
        std::cout << "ERROR: Requested bus number " << bus << " is larger than the max bus number " << (unsigned)max_bus << std::endl;
        throw std::exception();
    }

    // std::cout << "PCI config base addr:"<< std::hex << base_addr<< std::endl;
    base_addr += (bus * 1024 * 1024 + device * 32 * 1024 + function * 4 * 1024);
}


bool PciHandleM::exists(uint32 /* bus_*/, uint32 /* device_ */, uint32 /* function_ */)
{
    int handle = ::open("/dev/mem", O_RDWR);

    if (handle < 0) return false;

    ::close(handle);
    handle = ::open("/sys/firmware/acpi/tables/MCFG", O_RDONLY);

    if (handle < 0) return false;

    ::close(handle);
    return true;
}

int32 PciHandleM::read32(uint64 offset, uint32 * value)
{
    return ::pread(fd, (void *)value, sizeof(uint32), offset + base_addr);
}

int32 PciHandleM::write32(uint64 offset, uint32 value)
{
    return ::pwrite(fd, (const void *)&value, sizeof(uint32), offset + base_addr);
}

int32 PciHandleM::read64(uint64 offset, uint64 * value)
{
    return ::pread(fd, (void *)value, sizeof(uint64), offset + base_addr);
}

int32 PciHandleM::write64(uint64 offset, uint64 value)
{
    return ::pwrite(fd, (const void *)&value, sizeof(uint64), offset + base_addr);
}

PciHandleM::~PciHandleM()
{
    if (fd >= 0) ::close(fd);
}

#endif // PCM_USE_PCI_MM_LINUX

// mmaped I/O version

uint64 read_base_addr(int mcfg_handle, uint32 group_, uint32 bus)
{
    MCFGRecord record;
    int32 result = ::pread(mcfg_handle, (void *)&record, sizeof(MCFGRecord), sizeof(MCFGHeader) + sizeof(MCFGRecord) * group_);

    if (result != sizeof(MCFGRecord)) {
        ::close(mcfg_handle);
        throw std::exception();
    }

    const unsigned max_bus = (unsigned)record.endBusNumber ;

    if (bus > max_bus) {
        std::cout << "ERROR: Requested bus number " << bus << " is larger than the max bus number " << max_bus << std::endl;
        ::close(mcfg_handle);
        throw std::exception();
    }

    // std::cout << "DEBUG: PCI segment group="<<group_<<"number="<< record.PCISegmentGroupNumber << std::endl;
    return record.baseAddress;
}

PciHandleMM::PciHandleMM(uint32 groupnr_, uint32 bus_, uint32 device_, uint32 function_) :
    fd(-1),
    mmapAddr(NULL),
    bus(bus_),
    device(device_),
    function(function_),
    base_addr(0)
{
    int handle = ::open("/dev/mem", O_RDWR);

    if (handle < 0) throw std::exception();

    fd = handle;
    int mcfg_handle = ::open("/sys/firmware/acpi/tables/MCFG", O_RDONLY);

    if (mcfg_handle < 0) throw std::exception();

    if (groupnr_ == 0) {
        base_addr = read_base_addr(mcfg_handle, 0, bus);
    } else if (groupnr_ >= 0x1000) {
        // for SGI UV2
        MCFGHeader header;
        ::read(mcfg_handle, (void *)&header, sizeof(MCFGHeader));
        const unsigned segments = header.nrecords();

        for (unsigned int i = 0; i < segments; ++i) {
            MCFGRecord record;
            ::read(mcfg_handle, (void *)&record, sizeof(MCFGRecord));

            if (record.PCISegmentGroupNumber == 0x1000) {
                base_addr = record.baseAddress + (groupnr_ - 0x1000) * (0x4000000);
                break;
            }
        }

        if (base_addr == 0) {
            std::cout << "ERROR: PCI Segment 0x1000 not found." << std::endl;
            throw std::exception();
        }
    } else {
        std::cout << "ERROR: Unsupported PCI segment group number " << groupnr_ << std::endl;
        throw std::exception();
    }

    // std::cout << "DEBUG: PCI config base addr: 0x"<< std::hex << base_addr<< "for groupnr" << std::dec << groupnr_ << std::endl;
    ::close(mcfg_handle);
    base_addr += (bus * 1024 * 1024 + device * 32 * 1024 + function * 4 * 1024);
    mmapAddr = (char*) mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_SHARED , fd, base_addr);

    if (mmapAddr == MAP_FAILED) {
        std::cout << "mmap failed: errno is " << errno <<  std::endl;
        throw std::exception();
    }
}

bool PciHandleMM::exists(uint32 bus_, uint32 device_, uint32 function_)
{
    int handle = ::open("/dev/mem", O_RDWR);

    if (handle < 0) return false;

    ::close(handle);
    handle = ::open("/sys/firmware/acpi/tables/MCFG", O_RDONLY);

    if (handle < 0) return false;

    ::close(handle);
    return true;
}

int32 PciHandleMM::read32(uint64 offset, uint32 * value)
{
    *value = *((uint32*)(mmapAddr + offset));
    return sizeof(uint32);
}

int32 PciHandleMM::write32(uint64 offset, uint32 value)
{
    *((uint32*)(mmapAddr + offset)) = value;
    return sizeof(uint32);
}

int32 PciHandleMM::read64(uint64 offset, uint64 * value)
{
    *value = *((uint64*)(mmapAddr + offset));
    return sizeof(uint64);
}

int32 PciHandleMM::write64(uint64 offset, uint64 value)
{
    *((uint64*)(mmapAddr + offset)) = value;
    return sizeof(uint64);
}

PciHandleMM::~PciHandleMM()
{
    if (mmapAddr) munmap(mmapAddr, 4096);

    if (fd >= 0) ::close(fd);
}


#endif
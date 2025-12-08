# Little Penguin - Linux Kernel Development

![Language](https://img.shields.io/badge/language-C-blue)
![Platform](https://img.shields.io/badge/kernel-Linux%20Mainline-yellow)
![Focus](https://img.shields.io/badge/focus-Drivers%20%26%20Modules-red)
![Status](https://img.shields.io/badge/status-finished-success)

<br />
<p align="center">
  <h3 align="center">A journey into the Linux Kernel Source based on the Eudyptula Challenge</h3>
</p>

## 🗣️ About The Project

**Little Penguin** is a series of advanced kernel programming assignments designed to bridge the gap between user-space coding and **Kernel-space engineering**. Inspired by the *Eudyptula Challenge*, this project requires writing robust, compliant code that runs directly in ring 0.

Unlike standard application development, mistakes here mean system crashes (kernel panics). This project demonstrates proficiency in **Loadable Kernel Modules (LKM)**, **Device Drivers**, **Concurrency**, and **Internal Kernel Structures**.

---

## 📚 Technical Journey & Modules

This repository documents the progression from a basic module to complex interaction with kernel internals:

### 🔹 Core Fundamentals
* **Assignment 01 (Hello World):** Building a basic LKM, understanding `module_init`/`module_exit` macros and the kernel build system (`Kbuild`).
* **Assignment 02 (Kernel Build):** Compiling a custom kernel from source. Created a git patch to modify the `EXTRAVERSION` in the Makefile, understanding the kernel release cycle.
* **Assignment 03 (Coding Style):** Refactoring legacy code to strictly adhere to the **Linux Kernel Coding Style** (checked via `scripts/checkpatch.pl`).

### 🔹 Device Drivers & Hardware
* **Assignment 04 (USB Hotplugging):** Implemented a driver that automatically loads when a specific USB device is plugged in.
    * *Tech:* `MODULE_DEVICE_TABLE`, `usb_device_id`, and udev rules.
* **Assignment 05 (Misc Character Driver):** Created a character device `/dev/fortytwo`.
    * *Challenge:* Safely transferring data between User Space and Kernel Space using `copy_to_user` / `copy_from_user` to validate credentials (login check).

### 🔹 Internals & Concurrency
* **Assignment 07 (Debugfs & Locking):** Exposed internal kernel state via `/sys/kernel/debug/fortytwo/`.
    * *Jiffies:* Reading the system timer.
    * *Concurrency:* Implemented **Mutex Locking** (`mutex_lock`/`mutex_unlock`) to prevent race conditions when writing to a shared buffer from multiple threads.
* **Assignment 08 (Code Auditing):** Fixed a broken, messy module. Solved memory leaks (`kfree` missing), race conditions, and style violations.

### 🔹 Advanced Filesystem Traversal
* **Assignment 09 (Procfs & VFS):** The most complex module. It creates `/proc/mymounts` to list all mount points in the system.
    * *Internals:* Manually traversing the kernel's `struct mount` linked lists via `current->nsproxy->mnt_ns->root`.
    * *VFS:* Understanding `dentry`, `super_block`, and `vfsmount` structures to resolve paths efficiently.

---

## 🏗️ Workflow & Coding Standards

The Linux Kernel has one of the strictest coding standards in the world.
* **Checkpatch Compliant:** All code passed the `scripts/checkpatch.pl` script.
* **Memory Safety:** Strict resource management (no memory leaks allowed in kernel space).
* **Patch Submission:** Learned to create professional git patches for submitting changes, simulating real-world contribution workflows.

---

## 🚀 Installation & Usage

### Requirements
* A Linux environment (Virtual Machine recommended).
* Root privileges.
* Kernel headers installed.

### Build and Load a Module
~~~bash
# Clone the repository
git clone https://github.com/eandres83/little-penguin-1.git

# Enter a specific assignment (e.g., Assignment 09 - Procfs)
cd assignment_09

# Compile
make

# Load the module into the Kernel
sudo insmod main.ko

# Interact with the module
cat /proc/mymounts

# Check the Kernel Ring Buffer for logs
dmesg | tail

# Unload
sudo rmmod main
~~~

---
*Developed by Eleder Andres. Based on the Eudyptula Challenge structure.*

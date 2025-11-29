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

Unlike standard application development, mistakes here mean system crashes (kernel panics). This project demonstrates proficiency in **Loadable Kernel Modules (LKM)**, **Device Drivers**, and **Kernel Internals**.

### 🎯 Key Engineering Goals
- **Kernel Compilation:** Building custom kernels from Linus Torvalds' mainline tree and `linux-next` branches.
- **Device Drivers:** Implementing a **Misc Character Device** (`/dev/fortytwo`) with custom read/write handlers.
- **Hardware Interaction:** Creating modules that trigger automatically via **USB Hotplug** events (e.g., detecting a keyboard insertion).
- **Kernel Interfaces:** Exposing kernel data to user-space via **debugfs** and **procfs**.
- **Concurrency:** Managing `jiffies` timers and implementing proper locking mechanisms for thread safety.

---

## 🛠️ Technical Modules Implemented

### 1. The Character Device Driver
Implemented a "Misc Device" driver that registers dynamically in `/dev`.
* **Functionality:** It validates user input against a stored internal value (login) and handles error codes correctly.
* **Tech Stack:** `struct file_operations`, `misc_register`, `copy_to_user`, `copy_from_user`.

### 2. USB Hotplugging
A module designed to interact with the hardware subsystem.
* **Logic:** The module uses the device table ID to automatically load itself when a specific piece of hardware (USB Keyboard) is plugged into the machine.
* **Tools:** `MODULE_DEVICE_TABLE`, `usb_device_id`.

### 3. Debugfs & Concurrency
Created a debugging interface at `/sys/kernel/debug/fortytwo/` to monitor system internals.
* **Virtual Files created:**
    * `id`: Read/Write access to internal variables.
    * `jiffies`: Read-only access to the system timer (uptime tracking).
    * `foo`: A root-only writeable buffer protected by **mutex/spinlocks** to prevent race conditions during concurrent writes.

### 4. Mount Point Walker
A module that traverses the kernel's internal linked lists to display all mounted filesystems, exposed via `/proc/mymounts`.
* **Challenge:** Navigating internal kernel structures safely to list mount points matching the behavior of `mount`.

---

## 🏗️ Workflow & Coding Standards

The Linux Kernel has one of the strictest coding standards in the world.
* **Checkpatch Compliant:** All code passed the `scripts/checkpatch.pl` script to ensure compliance with the official **Linux Kernel Coding Style**.
* **Patch Submission:** Learned to create professional git patches for submitting changes to the Makefile (modifying `EXTRAVERSION`), simulating real-world contribution workflows.

---

## 🚀 Installation & Usage

### Requirements
* A Linux environment (Virtual Machine recommended).
* Root privileges.
* Kernel headers installed.

### Build and Load a Module
```bash
# Clone the repository
git clone [https://github.com/eandres83/little-penguin-1.git](https://github.com/eandres83/little-penguin-1.git)

# Enter a specific assignment (e.g., Assignment 05 - Misc Driver)
cd assignment_05

# Compile
make

# Load the module into the Kernel
sudo insmod main.ko

# Check the Kernel Ring Buffer for logs
dmesg | tail

# Interact with the device
echo "login" > /dev/fortytwo
cat /dev/fortytwo

# Unload
sudo rmmod main

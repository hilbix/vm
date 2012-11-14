VirtualBox bash helper scripts
==============================

This is for VirtualBox OSE 3.x.
Note that VirtualBox 4.x has a largely incompatible commandline interface.


First use:
----------

```bash
cd
git clone https://github.com/hilbix/vm
cd vm
cp .vm.conf.dist .vm.conf
vi .vm.conf
./.setup
```

If my personal settings do not fit for you,
fork the repo at GitHub and add a `.vm.conf`

You might consider to keep `.vm.conf` secret.


About:
------

Getting started with VirtualBox is easy.  However if you use the `virtualbox` GUI often on your server you will notice that certain settings do not fit your needs, or are tedious to enter.  Also you want to run your Guests headless usually.

To do this from commandline on the Host I do a checkout of this repo into `$HOME/vm/`.  Note that I am used to set `export LC_ALL=C` in `.bashrc` such that all the dot-files are sorted on top of the directory, leaving the auxiliary files on bottom to easily view everything with ls.

There are 4 classes of files:

* Auxiliary (Aux) files which are created and not managed with GIT.

* Helpers which you will not call yourself, starting with `.vm.`.  The configuration `.vm.conf` is a helper, too, which you can edit using vi or text editors.

* Scripts, which will run without arguments.

* Scripts, which require a list of VMs on the commandline.

The Aux files are named after the port number on which the VRDE server will run.  In the Aux files the first line is the name of the VM.  All remaining lines are not yet defined (later will contain settings).


Scripts without args:
---------------------

`./.setup`
	Automatically setup everything, can be rerun if new VMs show up

`./.list`
	List all available VMs

`./.port`
	Re-assign port numbers from the accessory files

`./.autostart`
	Call `.run` with the VM name taken from the filename.
	Link this to your convenience script like `ln -s ../vm/.autostart ~/autostart/"My VM Name"`
	With http://www.scylla-charybdis.com/tool.php/ptybuffer you then can call `~/bin/autostart.sh` to start the VMs on boot.


Scripts with VM arg:
--------------------

The argument can be the VM name or the port number.  Usually the port is quicker thanks to Bash file completion. (Is there a bash-completion for VirtualBox?)

`./.desk VM`
	Start rdesktop-vrdp for the given VM

`./.run VM`
	Start the VM in FOREGROUND in headless mode.
	This basically is thought for controlled shell usage, like sending out eMail when the VM crashes.

`./.start VM`
	Start the VM in BACKGROUND in headless mode.
	Afterwards you can call `.desk`

`./.show VM`
	`.start` and `.desk` combined

`./.hide VM`
	Kills background `.desk` processes for given VM (keeps the VM running)

`./.fix VM`
	Fix certain settings of a VM (according to .vm.conf)


Scripts with VM arg and confirmation:
-------------------------------------

`./.cad VM`
	Send a Ctrl-Alt-Del to the VM

`./.stop VM`
	Stop the VM.  This is: Short press of the power button.

`./.kill VM`
	Poweroff the VM, unconditionally.
	This is: Long press of the power button.


Scripts with various args:
--------------------------
These commands show a short the Usage if called without arguments.

`./.list h`
	List all registered harddisk images

`./.list m`
	List all registered media files (including harddisk images)

`./.list i`
	List all virtual network interfaces.  `./.list n` is the same.

`./.list o`
	Overview of all ostypes

`./.mk-hd-fixed SIZE:[PATH/]NAME...`
	SIZE is in MB.
	Create harddisk images (VDI), which are completely preallocated.
	This is highly recommended on servers, such that the resulting VDI-files are "defragmented".
	Also I recommend to use EXT4 with it (however I did no speed tests yet).
	In the SIZE:NAME case the Disk created is /VM/$USER/NAME/NAME.vdi,
	else it is /VM/$USER/PATH/NAME.vdi or PATH/NAME.vdi,
	in case that PATH is relative or absolute (starting with ./, ../ or /).

`./.mk-hd-dyn SIZE:[PATH/]NAME...`
	Like `./.mk-hd-fixed`, but instead creates an empty dynamically allocated VDI file.
	Note that all disks are registered with VirtualBox, so you can list them using `./.list h`

`./.mk-vm VMNAME ostype`
	Create a new VM with the given name and the given OS-type.
	See `./.list o` for possible OS-types.


Notes
======

As I start to use the Debian originated package virtualbox-ose, RDP-Suport will be dropped.  Instead VNC support is added.  VNC support is very different from RDP support, though.


Security
--------

Securing `VBoxHeadless` is not very easy, as it, per default, listens on every interface it can see if used with the `--vnc` option.  Apparently it is lacking an option to bind to a single IP.  The situation can be improved with a workaround, such that VBoxHeadless listens on localhost only.  However this workaround is a bit difficult to apply in case you want to run VBoxHeadless as an ordinary user and not root.

Following fix is automatically used by `.run` if it is installed:

```bash
cd
git clone https://github.com/hilbix/misc
cd force_local
make
sudo make install
cd ../wrap
make
sudo make install
sudo make example
```

This does following:

- Installs `force_local.so` into `/usr/local/lib/`
- Installs `wrap` into `/wrap/`
- Installs the example (which wraps VBoxHeadless) into `/wrap/`
- `.run` detects `/wrap/VBoxHeadless` and automatically uses it
- You can alter the wrapped runtime environment by editing `sudo vim /wrap/VBoxHeadless.wrap` easily


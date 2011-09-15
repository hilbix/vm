VirtualBox bash helper scripts
==============================

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
fork the repo at GitHub and add a .vm.conf

You might consider to keep .vm.conf secret.


About:
------

Getting started with VirtualBox is easy.  However if you use the `virtualbox` GUI
often on your server you will notice that certain settings do not fit your needs,
or are tedious to enter.  Also you want to run your Guests headless usually.

To more easily access my VMs on the commandline on the Host I created a directory
 $HOME/vm/
and placed several files into it.  Often I start such scripts with a dot to sort
them on top of the directory, leaving the auxiliary files on bottom to easily
view them with ls.

There are 4 classes of files:

* Auxiliary (Aux) files which are created and not managed with GIT.

* Helpers which you won't call yourself, starting with .vm.
The configuration is a helper, too, which you can edit using vi or text editors.

* Scripts which will run without arguments.

* Scripts which require a list of VMs on the commandline.

The Aux files are named after the port number on which the VRDE server will run.
In the Aux files the first line is the name of the VM.
All remaining lines are not yet defined (later will contain settings).


Scripts without args:
---------------------

./.setup
	Automatically setup everything, can be rerun if new VMs show up

./.list
	List all available VMs

./.port
	Re-assign port numbers from the accessory files


Scripts with VM args:
---------------------

The argument can be the VM name or the port number.  Usually the port is quicker
thanks to Bash file completion. (Anybody writes a customized Bash completion
for VirtualBox?)

./.desk VM
	Start rdesktop-vrdp for the given VM

./.run VM
	Start the VM in headless mode

./.fix VM
	Fix certain settings of a VM (according to .vm.conf)


Scripts with VM args and confirmation:
--------------------------------------

./.cad VM
	Send a Ctrl-Alt-Del to the VM

./.stop VM
	Stop the VM


#
# Common parts
#
# Overwrite my personal settings using .vm.conf

cd "`dirname "$0"`" || exit

export LC_ALL=C

. .vm.conf.dist
[ -f .vm.conf ] && . .vm.conf

[ -z DISPLAY ] && export DISPLAY=":0"

OOPS()
{
echo "BUG $?: $*" >&2
exit 1
}

# usage MINARGS MAXARGS USAGEstring "$@"
usage()
{
[ -z "$1" -o "$#" -ge "`expr "${1:-0}" + 3`" ] && [ -z "$2" -o "$#" -le "`expr "${2:-0}" + 3`" ] && return
echo "Usage: `basename "$BASH_SOURCE"` $3" >&2
exit 1
}

vbm()
{
VBoxManage -q "$@"
}

getvm2()
{
if	[ -f "$1" ]
then
	case "$1" in
	[1-9]*[0-9])	port="$1"
			read -r vm <"$port" && [ -n "$vm" ] && return
			;;
	esac
else
	name="`vbm showvminfo "$1" --machinereadable | sed -n 's/^name="//p'`"
	name="${name%\"}"
	[ -n "$name" ] || name="$1"
	for port in [1-9]*[0-9]
	do
		read -r vm <"$port"
		[ ".$vm" = ".$name" ] && return
	done
fi
false
}

getvm()
{
getvm2 "$1" || OOPS "wrong VM $1"
}

getvm3()
{
getvm2 "$1" || { vm="$1"; port=; }
vbm showvminfo "$vm" >/dev/null 2>&1 || OOPS "Unknown VM $vm"
}

confirm()
{
# Send NL for more easy parsing in case of remote control
echo "$*?"
read -r ans || OOPS eof
case "$ans" in
y)	return 0;;
esac
return 1
}

askvm()
{
getvm "$1"
shift
confirm "$* VM $vm" || OOPS no
}

ctrl()
{
vbm controlvm "$vm" "$@"
}

getvmvalue2()
{
vbm showvminfo "$vm" --machinereadable | awk -vPARM="$1" -F= '$1==PARM { sub(/^[^=]*=/,""); sub(/^"/,""); sub(/"$/,""); print; exit(0) }'
}
getvmvalue()
{
eval "FIX=\"\${FIX_$1}\""
vmval="`getvmvalue2 "${FIX:-$1}"`"
[ -z "$FIX" ] && return

# If we have fixed the key, perhaps we need to fix the value, too
tmpval="$vmval"
vmval=
eval "vmval=\"\${VAL_$1_$tmpval}\""
case "$vmval" in
'')	vmval="$tmpval";;
'%'*)	vmval="${vmval#%}";;	# % is substitute for nothing, use %% to start with %
esac
}

mod()
{
getvmvalue "$1"
if	[ ".$2" = ".$vmval" ]
then
	: echo "'$vm' var $1 was '$2'"
else
	echo "'$vm' set $1 to '$2' (was '$vmval')"
	vbm modifyvm "$vm" --"$@"
fi
}

# format variant "$@"
mkdisk()
{
usage 3 '' "SIZE_MB:VMNAME[/DISKNAME]" "$@"

format="$1"
variant="$2"
shift 2

for a
do
	size="${a%%:*}"
	[ 0 -lt "$size" ] || OOPS "format is NUMBER:NAME, you gave $a"
	name="${a#*:}"
	case "$name" in
	*.vdi)		OOPS "please do not add extension .vdi";;
	/*|./*|../*)	;;
	*/*)	name="$VMDATA/$name";;
	*)	name="$VMDATA/$name/$name";;
	esac
	dir="${name%/*}"
	[ -d "$dir" ] || mkdir "$dir" || OOPS "cannot create $dir"
	vbm createhd --remember --filename "$name.vdi" --size "$size" --format "$format" --variant "$variant"
done
}


# Fixes for getvmvalue

FIX_mouse="hidpointing"
VAL_mouse_ps2mouse="ps2"
FIX_vrdeport=vrdeports

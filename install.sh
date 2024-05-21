cmds="cdls cdp cdr cds"

create_temp_dir() {
	temp_dir=$(mktemp -d 2>/dev/null || mktemp -d -t 'temp_dir')
	printf "Created temp dir: $temp_dir\n"
}

remove_temp_dir() {
	rm -rf $temp_dir
}

build() {
	mkdir -p bin/
	for cmd in $cmds; do
		echo "Building $cmd..."
		gcc -o bin/$cmd src/$cmd.c src/cache/cache.c
	done
}

install() {
	read -p "Do you want to install the commands globally? (/usr/local/bin) [y/N] " -n 1 -r
	if [[ $REPLY =~ ^[Yy]$ ]]; then
		mkdir -p /usr/local/bin
		for cmd in $(ls bin/); do
			echo "Installing $cmd..."
			cp bin/$cmd /usr/local/bin
		done
	fi

	echo 1
}

uninstall() {
	read -p "Are you sure you want to delete the following commands from your system? $cmds [y/N] " -n 1 -r
	if [[ $REPLY =~ ^[Yy]$ ]]; then
		for cmd in $cmds; do
			echo "Uninstalling $cmd..."
			rm $(which $cmd)
		done
	fi
}

get_profile() {
	if [ -f ~/.zshrc ]; then
		echo ~/.zshrc
	elif [ -f ~/.bashrc ]; then
		echo ~/.bashrc
	elif [ -f ~/.bash_profile ]; then
		echo ~/.bash_profile
	elif [ -f ~/.profile ]; then
		echo ~/.profile
	else
		printf "Could not find a profile file. Please add the following line to your shell profile:\n"
		cat ./aliases
	fi
}

case $1 in
build)
	build
	;;
uninstall)
	uninstall
	;;
*)
	create_temp_dir
	curr=$(pwd)
	cd $temp_dir

	git clone https://github.com/meir/cdl2.git .
	build
	installed=$(install)

	if [ $installed -eq 1 ]; then
		profile=$(get_profile)
		read -p "Do you want to add the aliases to your shell profile? ($profile) [y/N] " -n 1 -r
		if [[ $REPLY =~ ^[Yy]$ ]]; then
			cat ./aliases >>$profile
		fi
	else
		printf "Cancelling installation...\n"
	fi

	cd $curr
	remove_temp_dir
	unset curr
	unset profile
	;;
esac

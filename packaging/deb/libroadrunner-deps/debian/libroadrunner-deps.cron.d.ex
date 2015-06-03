#
# Regular cron jobs for the libroadrunner-deps package
#
0 4	* * *	root	[ -x /usr/bin/libroadrunner-deps_maintenance ] && /usr/bin/libroadrunner-deps_maintenance

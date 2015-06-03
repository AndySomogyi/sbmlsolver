#
# Regular cron jobs for the libroadrunner package
#
0 4	* * *	root	[ -x /usr/bin/libroadrunner_maintenance ] && /usr/bin/libroadrunner_maintenance

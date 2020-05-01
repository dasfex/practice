find -L /etc -samefile '/etc/init.d/cron' 2>/dev/null | grep -v '^/etc/init.d/cron$'

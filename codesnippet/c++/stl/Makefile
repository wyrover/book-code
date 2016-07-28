all :
	for dir in `ls`; do	\
		if [ -d $$dir ]; then \
			$(MAKE) -C $$dir; \
		fi \
	done

clean :
	for dir in `ls`; do	\
		if [ -d $$dir ]; then \
			$(MAKE) clean -C $$dir; \
		fi \
	done

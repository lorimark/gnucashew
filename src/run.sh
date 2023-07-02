#ps aux | grep 8077 | grep artecams | awk '{print $2}' | xargs kill -9
# sleep 1

echo "running"
# valgrind -v --leak-check=full --show-leak-kinds=all \
# gdb --args \
./gnucashew-bin \
  --pid-file=gnucashew-bin.pid \
  --config=/opt/Wt4/etc/wt/wt_config.xml \
  --approot="approot" \
  --docroot="docroot;.,/images,/resources,/styles,/themes" \
  --errroot="errroot" \
  --http-listen 0.0.0.0:8080 \


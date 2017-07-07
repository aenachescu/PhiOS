# remove current 
if [ -d "build" ];
then
    rm -r -d "build"
fi

# get archive
wget -O build.tar.gz "https://onedrive.live.com/download?cid=9B17C13D0B35F82E&resid=9B17C13D0B35F82E%21401&authkey=AMivmxXl5pr8kzc"

# unpack
tar -zxvf build.tar.gz

# clean-up
rm build.tar.gz

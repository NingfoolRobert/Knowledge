########################################################################
# File Name:create.sh
# Author:Benin
# mail:nbf0537@163.com
# Created Time:2019年08月11日 星期日 21时49分55秒
#########################################################################
#!/bin/bash

protoc -I=./ --cpp_out=./ $0 


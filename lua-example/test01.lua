

local path = "?;?.lua;../?;../?.lua;" .. package.path
package.path = path
print(package.path)

local myio = require 'engine.io'
require 'engine.string'
local myformat = require 'engine.format'
require 'engine.table'
require 'engine.util'
require 'engine.base64'
require 'engine.sha1'
require 'engine.functional'
require 'engine.math'
require 'engine.dkjson'

if myio.fileExists('c:/sdkclient_1.0.0.510.zip') then
    print('1111111')
end

filesize = 9223372036854775807

print(myformat.fileSizeFormat(filesize))

printf("Hello %s from %s on %s\n", os.getenv"USER" or "there", _VERSION, os.date())


print(normalizePath("c:\\test.rar"))

print(dir_name(normalizePath('H:\\rover\\rover-self-work\\cpp\\book-code\\')))
print(dir_name(normalizePath('H:\\rover\\rover-self-work\\cpp\\book-code')))
print(base_name(normalizePath('H:\\rover\\rover-self-work\\cpp\\book-code\\')))
print(base_name(normalizePath('H:\\rover\\rover-self-work\\cpp\\book-code')))

# opennany

## usage

enables the `openconnect` program to handle anyconnect xml configuragtion files

it accepts one option, which is the anyconnect configuration file
```bash
anyconnect ~/profile.xml # for example
```
then it will list all the servers available and ask you to specify which one to connect

it's good enough handling utf-8 chars

n.b., the default limit of the servers is 36, if you have more than 36 servers, you may need to moldify the definition of the variable `al` in file `p.c`

## install

### release

download the binary from the release page

### build

this tool uses expat to handle xml, make sure you have it installed correctly

clone this repo and run `make`
```bash
git clone https://github.com/RadioNoiseE/opennany
cd opennany
make
```
then move the resulted binary to your `$PATH`
```bash
mv anyconnect $PATH
```

## contribution

fell free to fork your own version

## license

under mit license

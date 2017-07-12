Gcoin Core 1.2.1
=====================

Setup
---------------------
[Gcoin Core](https://github.com/OpenNetworking/gcoin-community) is the original Gcoin client and it builds the backbone of the network. However, it downloads and stores the entire history of Gcoin transactions; depending on the speed of your computer and network connection, the synchronization process can take anywhere from a few hours to a day or more.

### Need Help?

* See the documentation at the [Gcoin Wiki](https://github.com/OpenNetworking/gcoin-community/wiki)
for help and more information.
* Ask for help on the [Gcoin](https://groups.google.com/forum/#!forum/gcoins) forums.

Building
---------------------
The following are developer notes on how to build Gcoin on your native platform. They are not complete guides, but include notes on the necessary libraries, compile flags, etc.

- [Unix Build Notes](build-unix.md)

Development
---------------------
The Gcoin repo's [root README](https://github.com/OpenNetworking/gcoin-community/blob/develop/README.md) contains relevant information on the development process and automated testing.

- [Developer Notes](developer-notes.md)
- [Unit Tests](unit-tests.md)
- [Unauthenticated REST Interface](REST-interface.md)
- [BIPS](bips.md)

### Resources
* Discuss on the [Gcoin](https://groups.google.com/forum/#!forum/gcoins) forums.

### Miscellaneous
- [Files](files.md)
- [Init Scripts (systemd/upstart/openrc)](init.md)

License
---------------------
Distributed under the [MIT software license](http://www.opensource.org/licenses/mit-license.php).
This product includes software developed by the OpenSSL Project for use in the [OpenSSL Toolkit](https://www.openssl.org/). This product includes
cryptographic software written by Eric Young ([eay@cryptsoft.com](mailto:eay@cryptsoft.com)), and UPnP software written by Thomas Bernard. This
product includes software developed by the Crypto++ Project for use in the [Crypto++ Library](https://www.cryptopp.com).

Used in 1.2.1.1
---------------------
* wallet.dat: personal wallet (BDB) with keys and transactions
* peers.dat: peer IP address database (custom format); since bitcoin 0.7.0
* blkminer.dat: recent block miner database (custom format); since 1.1
* member.dat: alliance member database (custom format); since 1.1
* license.dat: license database (custom format); since 1.1
* miner.dat: miner database (custom format); since 1.2
* blocks/blk000??.dat: block data (custom, 128 MiB per file); since bitcoin 0.8.0
* blocks/rev000??.dat; block undo data (custom); since bitcoin 0.8.0 (format changed since pre-0.8)
* blocks/index/*; block index (LevelDB); since bitcoin 0.8.0
* chainstate/*; block chain state database (LevelDB); since bitcoin 0.8.0
* database/*: BDB database environment; only used for wallet since bitcoin 0.8.0

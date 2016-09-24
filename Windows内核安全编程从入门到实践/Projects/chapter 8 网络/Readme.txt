GetNDISOpenBlock:
1.注册一个假协议
要能通过协议的合法性检查，另外，注册后一些回调函数会被调用，这样某些回调必须“实现”。


2.直接获取
小端口驱动程序注册的时候，会将代表自身的ndis!_NDIS_M_DRIVER_BLOCK结构保存在drvobj中。
使用这种方法需要注意的是：（系统）遍历NDIS_M_DRIVER_BLOCK结构使用的是单向链表，很可能无法遍历完全，所以这里采用的方
法是遍历系统中的所有驱动对象，即\driver目录下的项，然后根据特征值0x4E4D4944（如果是小端口驱动的话）排除。这里带来
的一个问题是，在如Win7的系统中，如何找到\driver对象指针？答案是，仍然使用ObReferenceObjectByName这个函数，只是
其中的参数ObjectType需要我们来获取（没有导出ObpDirectoryObjectType），给出的办法是，调用Win7提供的ObGetObjectType,
参数为任意一个目录的指针。问题是，如何找一个任意的目录指针呢？比如可以ZwCreateFile(xxx),但这样又需要调用
ObreferenceObjectByHandle函数，而调用这个函数又将需要ObjectType……所以，最终办法是，提供一个假的目录对象，还好
ObGetObjectType比较单纯，直接提取Type值，而没有进行任何其他的检查。
另外，目录对象结构中的OBJECT_DIRECTORY_ENTRY代表一个子项，这里也就是我们需要的驱动对象了。
具体看代码。

from pyzenlib import FileIndex, ZenParser, oCWorldData, FileVersion, zCVobData, EVobType

FileIndex.initVDFS("D:\\Steam\\steamapps\\common\\TheChroniclesOfMyrtana\\Data")
vdf = FileIndex()
vdf.loadVDF("D:\\Steam\\steamapps\\common\\TheChroniclesOfMyrtana\\Data\\KM_Worlds.mod", "")
vdf.finalizeLoad()

parser = ZenParser("ARCHOLOS_MAINLAND.ZEN", vdf)
print(parser.getFileSize())
parser.readHeader()
print(parser.getZenHeader().objectCount)

world = oCWorldData()
parser.readWorld(world, FileVersion.Gothic2)


def readVob(vob: zCVobData):
    if vob.vobType == EVobType.VT_oCItem and vob.oCItem.instanceName == "ITPL_STRENGTH_HERB_01":
        print(vob.oCItem.instanceName)
    for childVob in vob.childVobs:
        readVob(childVob)


for rootVob in world.rootVobs:
    readVob(rootVob)

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <vdfs/fileIndex.h>
#include <zenload/zenParser.h>

namespace py = pybind11;

PYBIND11_MODULE(pyzenlib, m)
{
    m.doc() = R"pbdoc(
        Python bindings to ZenLib.
        -----------------------

        .. currentmodule:: pyzenlib

        .. autosummary::
           :toctree: _generate

           add
           subtract
    )pbdoc";
    // math
    py::class_<ZMath::float3>(m, "float3")
        .def_readonly("x", &ZMath::float3::x)
        .def_readonly("y", &ZMath::float3::y)
        .def_readonly("z", &ZMath::float3::z);

    // ZenLoad
    py::enum_<ZenLoad::ZenParser::FileVersion>(m, "FileVersion")
        .value("Gothic1", ZenLoad::ZenParser::FileVersion::Gothic1)
        .value("Gothic2", ZenLoad::ZenParser::FileVersion::Gothic2)
        .export_values();

    py::class_<ZenLoad::ZenParser>(m, "ZenParser")
        .def(py::init<const std::string&, const VDFS::FileIndex&>())
        .def("readHeader", static_cast<void (ZenLoad::ZenParser::*)()>(&ZenLoad::ZenParser::readHeader))
        .def("readWorld", static_cast<void (ZenLoad::ZenParser::*)(ZenLoad::oCWorldData&, ZenLoad::ZenParser::FileVersion)>(&ZenLoad::ZenParser::readWorld))
        .def("getFileSize", &ZenLoad::ZenParser::getFileSize)
        .def("getZenHeader", &ZenLoad::ZenParser::getZenHeader);

    py::class_<ZenLoad::ZenParser::ZenHeader>(m, "ZenHeader")
        .def(py::init<>())
        .def_readwrite("objectCount", &ZenLoad::ZenParser::ZenHeader::objectCount);

    py::class_<ZenLoad::oCWorldData>(m, "oCWorldData")
        .def(py::init<>())
        .def_readonly("rootVobs", &ZenLoad::oCWorldData::rootVobs);

    // ZenLoad - zCVobData and co
    py::enum_<ZenLoad::zCVobData::EVobType>(m, "EVobType")
        .value("VT_Unknown", ZenLoad::zCVobData::EVobType::VT_Unknown)
        .value("VT_zCVob", ZenLoad::zCVobData::EVobType::VT_zCVob)
        .value("VT_zCVobLevelCompo", ZenLoad::zCVobData::EVobType::VT_zCVobLevelCompo)
        .value("VT_oCItem", ZenLoad::zCVobData::EVobType::VT_oCItem)
        .value("VT_oCMOB", ZenLoad::zCVobData::EVobType::VT_oCMOB)
        .value("VT_oCMobInter", ZenLoad::zCVobData::EVobType::VT_oCMobInter)
        .value("VT_oCMobDoor", ZenLoad::zCVobData::EVobType::VT_oCMobDoor)
        .value("VT_oCMobBed", ZenLoad::zCVobData::EVobType::VT_oCMobBed)
        .value("VT_oCMobFire", ZenLoad::zCVobData::EVobType::VT_oCMobFire)
        .value("VT_oCMobLadder", ZenLoad::zCVobData::EVobType::VT_oCMobLadder)
        .value("VT_oCMobSwitch", ZenLoad::zCVobData::EVobType::VT_oCMobSwitch)
        .value("VT_oCMobWheel", ZenLoad::zCVobData::EVobType::VT_oCMobWheel)
        .value("VT_oCMobContainer", ZenLoad::zCVobData::EVobType::VT_oCMobContainer)
        .value("VT_zCVobLight", ZenLoad::zCVobData::EVobType::VT_zCVobLight)
        .value("VT_zCVobSound", ZenLoad::zCVobData::EVobType::VT_zCVobSound)
        .value("VT_zCVobSoundDaytime", ZenLoad::zCVobData::EVobType::VT_zCVobSoundDaytime)
        .value("VT_oCZoneMusic", ZenLoad::zCVobData::EVobType::VT_oCZoneMusic)
        .value("VT_oCZoneMusicDefault", ZenLoad::zCVobData::EVobType::VT_oCZoneMusicDefault)
        .value("VT_zCMessageFilter", ZenLoad::zCVobData::EVobType::VT_zCMessageFilter)
        .value("VT_zCCodeMaster", ZenLoad::zCVobData::EVobType::VT_zCCodeMaster)
        .value("VT_zCTrigger", ZenLoad::zCVobData::EVobType::VT_zCTrigger)
        .value("VT_zCTriggerList", ZenLoad::zCVobData::EVobType::VT_zCTriggerList)
        .value("VT_zCTriggerScript", ZenLoad::zCVobData::EVobType::VT_zCTriggerScript)
        .value("VT_oCTriggerChangeLevel", ZenLoad::zCVobData::EVobType::VT_oCTriggerChangeLevel)
        .value("VT_oCTriggerWorldStart", ZenLoad::zCVobData::EVobType::VT_oCTriggerWorldStart)
        .value("VT_zCMover", ZenLoad::zCVobData::EVobType::VT_zCMover)
        .value("VT_zCVobStartpoint", ZenLoad::zCVobData::EVobType::VT_zCVobStartpoint)
        .value("VT_zCVobSpot", ZenLoad::zCVobData::EVobType::VT_zCVobSpot)
        .value("VT_zCPFXControler", ZenLoad::zCVobData::EVobType::VT_zCPFXControler)
        .value("VT_oCTouchDamage", ZenLoad::zCVobData::EVobType::VT_oCTouchDamage)
        .value("VT_zCTriggerUntouch", ZenLoad::zCVobData::EVobType::VT_zCTriggerUntouch)
        .value("VT_zCMoverControler", ZenLoad::zCVobData::EVobType::VT_zCMoverControler)
        .value("VT_zCVobLightPreset", ZenLoad::zCVobData::EVobType::VT_zCVobLightPreset)
        .export_values();

    py::class_<decltype(ZenLoad::zCVobData::oCItem)>(m, "oCItem")
        .def_readonly("instanceName", &decltype(ZenLoad::zCVobData::oCItem)::instanceName);

    py::class_<decltype(ZenLoad::zCVobData::oCMobLockable)>(m, "oCMobLockable")
        .def_readonly("contains", &decltype(ZenLoad::zCVobData::oCMobLockable)::contains)
        .def_readonly("locked", &decltype(ZenLoad::zCVobData::oCMobLockable)::locked)
        .def_readonly("keyInstance", &decltype(ZenLoad::zCVobData::oCMobLockable)::keyInstance)
        .def_readonly("pickLockStr", &decltype(ZenLoad::zCVobData::oCMobLockable)::pickLockStr);

    py::class_<decltype(ZenLoad::zCVobData::oCMOB)>(m, "oCMOB")
        .def_readonly("focusName", &decltype(ZenLoad::zCVobData::oCMOB)::focusName)
        .def_readonly("damage", &decltype(ZenLoad::zCVobData::oCMOB)::damage)
        .def_readonly("hitpoints", &decltype(ZenLoad::zCVobData::oCMOB)::hitpoints);

    py::class_<ZenLoad::zCVobData>(m, "zCVobData")
        .def(py::init<>())
        .def_readonly("vobType", &ZenLoad::zCVobData::vobType)
        .def_readonly("vobName", &ZenLoad::zCVobData::vobName)
        .def_readonly("visual", &ZenLoad::zCVobData::visual)
        .def_readonly("position", &ZenLoad::zCVobData::position)
        .def_readonly("vobObjectID", &ZenLoad::zCVobData::vobObjectID)
        .def_readonly("childVobs", &ZenLoad::zCVobData::childVobs)
        .def_readonly("oCItem", &ZenLoad::zCVobData::oCItem)
        .def_readonly("oCMobLockable", &ZenLoad::zCVobData::oCMobLockable)
        .def_readonly("oCMOB", &ZenLoad::zCVobData::oCMOB);

    py::class_<VDFS::FileIndex>(m, "FileIndex")
        .def(py::init<>())
        .def_static("initVDFS", &VDFS::FileIndex::initVDFS)
        .def("finalizeLoad", &VDFS::FileIndex::finalizeLoad)
        .def("loadVDF", static_cast<bool (VDFS::FileIndex::*)(const std::u16string&, const std::string&)>(&VDFS::FileIndex::loadVDF));

    m.attr("__version__") = "dev";
}

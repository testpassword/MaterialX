//
// TM & (c) 2021 Lucasfilm Entertainment Company Ltd. and Lucasfilm Ltd.
// All rights reserved.  See LICENSE.txt for license.
//

#include <MaterialXFormat/XmlExport.h>

#include <MaterialXFormat/Util.h>

namespace MaterialX
{

namespace
{

void mergeLooks(DocumentPtr doc, const XmlExportOptions* exportOptions)
{
    if (exportOptions && exportOptions->mergeLooks)
    {
        doc->mergeLooks(exportOptions->lookGroupToMerge);
    }
}

} // anonymous namespace

//
// XmlExportOptions methods
//

XmlExportOptions::XmlExportOptions() :
    XmlWriteOptions(),
    mergeLooks(true),
    flattenFilenames(true),
    modifyInPlace(true)
{
}

//
// Exporting
//

void exportToXmlStream(DocumentPtr doc, std::ostream& stream, const XmlExportOptions* exportOptions)
{
    DocumentPtr exportDoc = doc;
    if (exportOptions && !exportOptions->modifyInPlace)
    {
        exportDoc = doc->copy();
    }

    mergeLooks(exportDoc, exportOptions);
    if (exportOptions)
    {
        if (exportOptions->libraries)
        {
            exportDoc->importLibrary(exportOptions->libraries);
        }
        if (exportOptions->flattenFilenames)
        {
            flattenFilenames(exportDoc, exportOptions->resolvedTexturePath, exportOptions->stringResolver, exportOptions->skipFlattening);
        }
        for (ExportResolverPtr exportResolver : exportOptions->exportResolvers)
        {
            exportResolver->resolve(exportDoc);
        }
    }
    writeToXmlStream(exportDoc, stream, exportOptions);
}

void exportToXmlFile(DocumentPtr doc, const FilePath& filename, const XmlExportOptions* exportOptions)
{
    DocumentPtr exportDoc = doc;
    if (exportOptions && !exportOptions->modifyInPlace)
    {
        exportDoc = doc->copy();
    }

    mergeLooks(exportDoc, exportOptions);
    if (exportOptions)
    {
        if (exportOptions->libraries)
        {
            exportDoc->importLibrary(exportOptions->libraries);
        }
        if (exportOptions->flattenFilenames)
        {
            flattenFilenames(exportDoc, exportOptions->resolvedTexturePath, exportOptions->stringResolver, exportOptions->skipFlattening);
        }
        for (ExportResolverPtr exportResolver : exportOptions->exportResolvers)
        {
            exportResolver->resolve(exportDoc);
        }
    }
    writeToXmlFile(exportDoc, filename, exportOptions);
}

string exportToXmlString(DocumentPtr doc, const XmlExportOptions* exportOptions)
{
    DocumentPtr exportDoc = doc;
    if (exportOptions && !exportOptions->modifyInPlace)
    {
        exportDoc = doc->copy();
    }

    mergeLooks(exportDoc, exportOptions);
    if (exportOptions)
    {
        if (exportOptions->libraries)
        {
            exportDoc->importLibrary(exportOptions->libraries);
        }
        if (exportOptions->flattenFilenames)
        {
            flattenFilenames(exportDoc, exportOptions->resolvedTexturePath, exportOptions->stringResolver, exportOptions->skipFlattening);
        }
        for (ExportResolverPtr exportResolver : exportOptions->exportResolvers)
        {
            exportResolver->resolve(exportDoc);
        }
    }
    return writeToXmlString(exportDoc, exportOptions);
}

} // namespace MaterialX
$Cwd = Split-Path $MyInvocation.MyCommand.Path
$Cwd += "\"

<#
    RenderEngine Code Build Script
#>

$OutputFileName = "Engine.exe"

$Compiler = "g++"

$CompilerFlags = "-std=c++20"
$CompilerFlags += " -Wall"
$CompilerFlags += " -Wextra"
$CompilerFlags += " -Wpedantic"
$CompilerFlags += " -I Source\"

$CompilerFlags += " -ggdb"
$CompilerFlags += " -O0"

$LinkerFlags = "-municode"
$LinkerFlags += " -L."

Get-ChildItem -Recurse "Source\" -Include *.c, *.cpp | ForEach-Object {
    $SourceFileFullName = $_.FullName
    $SourceFileFullName = $SourceFileFullName.Replace($Cwd, "")
 
    $BuildPath = $_.DirectoryName.Replace($Cwd, "Temp\")
    if (-not (Test-Path -Path $BuildPath)) {
        New-Item -ItemType Directory -Path $BuildPath | Out-Null
    }

    $ObjectFileFullName = $BuildPath + "\" + $_.Name + ".o"

    Write-Host "Building C++ object $ObjectFileFullName"
    Invoke-Expression "$Compiler $CompilerFlags -c $SourceFileFullName -o $ObjectFileFullName"
}

$ObjectFileFullNameList = Get-ChildItem -Recurse "Temp\" -Filter *.o
$ObjectFileFullNameList = $ObjectFileFullNameList.FullName -join " "

$OutputPath = ($Cwd + "Bin\")
if (-not (Test-Path -Path $OutputPath)) {
    New-Item -ItemType Directory -Path $OutputPath | Out-Null
}

$OutputFileFullName = $OutputPath + $OutputFileName

Write-Host "Linking C++ executable $OutputFileFullName"
Invoke-Expression "$Compiler $ObjectFileFullNameList -o $OutputFileFullName $LinkerFlags"
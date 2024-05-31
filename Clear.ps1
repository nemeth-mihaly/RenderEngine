$Cwd = Split-Path $MyInvocation.MyCommand.Path
$Cwd += "\"

<#
    RenderEngine Code CleanUp Script
#>

$OutputFileName = "App.exe"

if (Test-Path -Path ("Bin\" + $OutputFileName)) {
    Remove-Item -Path ("Bin\" + $OutputFileName) -Force
}

if (Test-Path -Path ($Cwd + "Temp\")) {
    Remove-Item -Path ($Cwd + "Temp\") -Recurse -Force
}
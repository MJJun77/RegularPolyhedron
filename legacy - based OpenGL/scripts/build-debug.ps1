$ErrorActionPreference = 'Stop'

$repoRoot = Split-Path -Parent $PSScriptRoot
$buildDirectory = Join-Path $repoRoot 'build'
$vswhere = Join-Path ${env:ProgramFiles(x86)} 'Microsoft Visual Studio\Installer\vswhere.exe'
$cmakeRelativePath = 'Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe'

if (-not (Test-Path -LiteralPath $vswhere)) {
    throw 'Install Visual Studio 2022 or its Build Tools with Desktop development with C++.'
}

$visualStudio = & $vswhere -latest -products '*' -version '[17.0,18.0)' -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath

if (-not $visualStudio) {
    throw 'Visual Studio 2022 with the MSVC x64 build tools was not found.'
}

$cmake = Join-Path $visualStudio $cmakeRelativePath
if (-not (Test-Path -LiteralPath $cmake)) {
    $cmake = (Get-Command cmake.exe -ErrorAction SilentlyContinue).Source
}

if (-not $cmake) {
    throw 'Install C++ CMake tools for Windows in Visual Studio 2022, or add cmake.exe to PATH.'
}

& $cmake -S $repoRoot -B $buildDirectory -G 'Visual Studio 17 2022' -A x64
if ($LASTEXITCODE -ne 0) {
    exit $LASTEXITCODE
}

& $cmake --build $buildDirectory --config Debug --parallel
exit $LASTEXITCODE

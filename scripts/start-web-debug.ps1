$ErrorActionPreference = 'Stop'

$repoRoot = Split-Path -Parent $PSScriptRoot
$node = (Get-Command node.exe -ErrorAction SilentlyContinue).Source

if (-not $node) {
    $toolsDirectory = Join-Path $repoRoot 'build\web-tools'
    $portableNode = Get-ChildItem -LiteralPath $toolsDirectory -Directory -Filter 'node-*-win-*' -ErrorAction SilentlyContinue |
        ForEach-Object { Join-Path $_.FullName 'node.exe' } |
        Where-Object { Test-Path -LiteralPath $_ -PathType Leaf } |
        Select-Object -First 1
    $node = $portableNode
}

if (-not $node) {
    throw 'Install Node.js 22.12 or later, then run npm ci from the repository root.'
}

$vite = Join-Path $repoRoot 'node_modules\vite\bin\vite.js'
if (-not (Test-Path -LiteralPath $vite -PathType Leaf)) {
    throw 'Run npm ci from the repository root to install the web dependencies.'
}

# Keep the ready URL free of terminal escape sequences for the VS Code task matcher.
$env:NO_COLOR = '1'
Set-Location -LiteralPath $repoRoot
& $node $vite --host 127.0.0.1 --port 5173 --strictPort
exit $LASTEXITCODE

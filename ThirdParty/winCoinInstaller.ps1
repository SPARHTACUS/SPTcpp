# Script para: 1. download de nós de bibliotecas COIN-OR; 2. extrair os arquivos no diretório "\COIN"; 3. alterar linhas de arquivos de cabeçalho (quando necessário) compatibilizando com <windows.h>

Add-Type -AssemblyName System.IO.Compression.FileSystem

function Unzip{
    param([string]$zipfile, [string]$inpath, [string]$indir, [string]$outpath)

    $zipfileModule = [System.IO.Compression.ZipFile]::Open($zipfile, 'Read')
    $zipfileModule.Entries | ? { $_.FullName -like "$($inpath -replace '\\','/')/*.*" } | % {
        $file    = Join-Path $outpath $_.FullName
        $fileCur = $file.Replace($inpath,$indir)
        $parent = Split-Path -Parent $fileCur
        if (-not (Test-Path -LiteralPath $parent)){ New-Item -Path $parent -Type Directory | Out-Null }
        if ($_.Name -ne '') { [System.IO.Compression.ZipFileExtensions]::ExtractToFile($_, $fileCur, $true) }
    }
    $zipfileModule.Dispose()
}


$coinPath = $PSScriptRoot + '\COIN'

if (-not (Test-Path -LiteralPath $coinPath)){ New-Item -Path $coinPath -Type Directory | Out-Null }

$ClpZipFile = $coinPath + "\Clp.zip"
(New-Object System.Net.WebClient).DownloadFile('https://github.com/coin-or/Clp/archive/cef2d02f52eb5d99f1d63144bf00ebd878295380.zip', $ClpZipFile)
Unzip $ClpZipFile 'Clp-cef2d02f52eb5d99f1d63144bf00ebd878295380' 'Clp' $coinPath
Remove-Item -Path $ClpZipFile

$PathClpSimplexOther = $coinPath + '\Clp\src\ClpSimplexOther.hpp'
$filecontent = Get-Content -Path $PathClpSimplexOther
$filecontent[243] = $filecontent[243] -replace 'small' , 'small_'
Set-Content -Path $PathClpSimplexOther -Value $filecontent

$CbcZipFile = $coinPath + "\Cbc.zip"
(New-Object System.Net.WebClient).DownloadFile('https://github.com/coin-or/Cbc/archive/83abbb8d4e7d54ae23b208ec4abcee35a5bbc78e.zip',  $CbcZipFile)
Unzip $CbcZipFile 'Cbc-83abbb8d4e7d54ae23b208ec4abcee35a5bbc78e' 'Cbc' $coinPath
Remove-Item -Path $CbcZipFile

$CglZipFile = $coinPath + "\Cgl.zip"
(New-Object System.Net.WebClient).DownloadFile('https://github.com/coin-or/Cgl/archive/2ad1324397d32e8c63e9c7680c64469d0db45391.zip',  $CglZipFile)
Unzip $CglZipFile 'Cgl-2ad1324397d32e8c63e9c7680c64469d0db45391' 'Cgl' $coinPath
Remove-Item -Path $CglZipFile

$PathCglGMI = $coinPath + '\Cgl\src\CglGMI\CglGMI.hpp'
$filecontent = Get-Content -Path $PathCglGMI
$filecontent[86] = $filecontent[86] -replace 'std::max' , '(std::max)'
Set-Content -Path $PathCglGMI -Value $filecontent

$PathCglGMI = $coinPath + '\Cgl\src\CglGMI\CglGMI.hpp'
$filecontent = Get-Content -Path $PathCglGMI
$filecontent[100] = $filecontent[100] -replace 'std::max' , '(std::max)'
Set-Content -Path $PathCglGMI -Value $filecontent

$OsiZipFile = $coinPath + "\Osi.zip"
(New-Object System.Net.WebClient).DownloadFile('https://github.com/coin-or/Osi/archive/46ca62586ba5c956c1af88dfbdc74977c1c51b22.zip', $OsiZipFile)
Unzip $OsiZipFile 'Osi-46ca62586ba5c956c1af88dfbdc74977c1c51b22' 'Osi' $coinPath
Remove-Item -Path $OsiZipFile

$CoinUtilsZipFile = $coinPath + "\CoinUtils.zip"
(New-Object System.Net.WebClient).DownloadFile('https://github.com/coin-or/CoinUtils/archive/754cd74ae395ff92aa4a6e990aa438d53aab2807.zip', $CoinUtilsZipFile)
Unzip $CoinUtilsZipFile 'CoinUtils-754cd74ae395ff92aa4a6e990aa438d53aab2807' 'CoinUtils' $coinPath
Remove-Item -Path $CoinUtilsZipFile
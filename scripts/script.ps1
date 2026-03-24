# Ścieżka, w której chcesz stworzyć folder "algorithms"
$basePath = "C:\Users\jakub\Desktop\hash_comparer\algorithms"

try {
    # Tworzy folder nadrzędny jeśli nie istnieje
    if (-not (Test-Path $basePath)) {
        New-Item -Path $basePath -ItemType Directory -Force
        Write-Host "Utworzono folder nadrzędny: $basePath"
    }

    # Lista wszystkich algorytmów (podfolderów)
    $hashFolders = @(
        "auto_detect","md5","sha1","sha224","sha256","sha384","sha512",
        "sha512_256","sha512_224","sha3_224","sha3_256","sha3_384","sha3_512",
        "blake2b","blake2s","blake3","crc32","crc64","ripemd160","whirlpool",
        "keccak256","keccak512","xxhash32","xxhash64","xxhash3","adler32",
        "tiger","gost","streebog256","streebog512"
    )

    # Tworzenie podfolderów
    foreach ($folder in $hashFolders) {
        $fullPath = Join-Path $basePath $folder
        if (-not (Test-Path $fullPath)) {
            New-Item -Path $fullPath -ItemType Directory -Force
            Write-Host "Utworzono folder: $fullPath"
        } else {
            Write-Host "Folder już istnieje: $fullPath"
        }
    }
}
catch {
    Write-Host "Wystąpił błąd: $_" -ForegroundColor Red
}

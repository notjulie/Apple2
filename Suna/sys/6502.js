
function jsr(address) {
	return [
		0x20,
		address & 0xFF,
		address >> 8
	];
}

function lda_abs(address) {
	return [
		0xAD,
		address & 0xFF,
		address >> 8
	];
}

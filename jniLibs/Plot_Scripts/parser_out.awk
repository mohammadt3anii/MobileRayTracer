BEGIN {
	FS = " ";
	n = 0;
	sum = 0;
}

/Time in secs/{
	sum += $7;
	temp = $7;
	for (i = 0; i < n; i++) {
		if(temp < array[i]) {
			aux = array[i];
			array[i] = temp;
			temp = aux;
		}
	}
	array[n++]=temp;
	#print $0
}

END {
	sum = 0;
	for (i = 0; i < n; i++) {
		sum += array[i];
		#print "array["i"] = " array[i];
	}
	print threads ", " sum;
}
let () = 
	let n = ref 0
	and p = ref 0.
	and e_list = ref [] 
	and aux (i,j,l) =
		Printf.printf "%d %d %d \n" i j l 
	in begin
		print_string "input : n = ? ";
		n := read_int ();
		print_string "input : p = ? ";
		p := read_float ();
		for i = 0 to !n-1 do
			for j = i+1 to !n-1 do
				if Random.float 1. < !p then
					e_list := (i,j,1 + Random.int 10)::!e_list
			done
		done;
		Printf.printf "%d %d \n" !n (List.length !e_list);
		List.iter aux !e_list;
	end

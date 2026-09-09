for i in {1..4}; do
        echo "thread $i"
        for j in {1..10}; do
                time ./ompdct2 "$i"
        done
done

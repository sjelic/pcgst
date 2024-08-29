import os
import sys

def export_pdf(fn: str, nl: int, od: str = '.\images')-> None:
    if os.path.isdir(od):
        fnp = os.path.join(od,fn)
        if os.path.isfile(fnp):
            for i in range(int(nl)):
                # os.system(f"draw.io {fnp} --export --format pdf --layers {','.join(map(lambda x: str(x), range(i+1)))} --output {os.path.join(od, fn[:-7]+ '_' + str(i).zfill(2) + '.pdf')}")
                print(f"draw.io {fnp} --export --format pdf --layers {','.join(map(lambda x: str(x), range(i+1)))} --output {os.path.join(od, fn[:-7]+ '_' + str(i).zfill(2) + '.pdf')}")
        else:
            raise FileNotFoundError()
    else:
        raise NotADirectoryError()


if __name__ == '__main__':
    export_pdf(sys.argv[1], sys.argv[2], sys.argv[3])

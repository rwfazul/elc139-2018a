## O que é Chapel?

Uma lingaguem de programação paralela moderna (e _open-souce_) baseada em Java, C/C++, Python e Matlab. Visa aumentar a produtividade no desenvolvimento de programas concorrentes e paralelos (vários tipos de paralelismo em um único programa de maneira simples), sendo portável (roda em notebooks com diferentes SOs, _clusters_, _cloud_ e sistemas de HPC) e escalável (sistemas de memória distribúida, etc). 

## Instalação

Baseado no [guia oficial](https://chapel-lang.org/docs/1.14/usingchapel/QUICKSTART.html) de instalação:

+ Alguns pré-requisitos básicos podem ser vistos [aqui](https://chapel-lang.org/docs/1.14/usingchapel/prereqs.html) (a maioria são pacotes básicos que muitas distribuições _Unix-like_ já possuem por default).

+ O download do _.tar_ contendo o código fonte pode ser feito [aqui](https://chapel-lang.org/download.html) (atualmente na versão 1.17.1).

Em seguida:
 
``` bash
	$ tar -xzf chapel-1.17.1.tar.gz && cd chapel-1.17.1 && source util/quickstart/setchplenv.bash
```

O _build_ e a verificação da instalação são realizados com um arquivo Makefile:

``` make
	$ make && make check
```

Se tudo correu bem: SUCCESS: 'make check' passed!

## Compilação e execução

Básico:

``` bash
	$ chpl -o file file.chpl
	$ ./file
```

- Compilar com a flag '<em>--fast</em>' ativa uma série de otimizações;
- Se no programa estiver definido uma variável de configuração (ex. <em>config const name = "value";</em>), é possível setá-la com novos valores.
	- em tempo de compilação:
	
``` bash
	$ chpl -o file file.chpl -s name="novoValor"
```

	- em tempo de execução: 
	
``` bash
	$ ./file --name="novoValor"
``` 

## Descobrindo a linguagem

Os testes que estão sendo realizados podem ser vistos nos arquivos <em>*.chpl</em>.

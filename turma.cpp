    Turma::Turma() = default;

    Turma::Turma(int id, int disciplinaId, const std::string &codigoTurma, int professorId)
        : id(_id), disciplinaId(_disciplinaId), codigoTurma(_codigoTurma), professorId(_professorId){}

    int Turma::getId() const{ return id; }
    int Turma::getDisciplinaId() const{ return disciplinaId; }
    std::string Turma::getCodigoTurma() const{ return codigoTurma; }
    int Turma::getProfessorId() const{ return professorId; }
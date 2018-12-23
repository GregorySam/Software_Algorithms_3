//
// Created by greg on 23/12/2018.
//

#include <cmath>
#include "User.h"



User::User(unsigned int bitcoins_num)
{
    static unsigned int id=0;

    user_id=id;
    cc_scores.resize(bitcoins_num,nan(""));

    id++;
}